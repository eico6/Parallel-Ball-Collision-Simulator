# Parallel-Bouncing-Balls
- INF236 (Parallel Programming) course at UiB (Univeristy of Bergen), compulsory 3.
- by Eivind Hobrad Naasen & Daniel Ngyuen

## Development notes
- C++ version: g++.exe (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders, r8) 13.2.0
- Graphics library: raylib version 5.5


## TEMP

Java code:

# Main.java

```
package com.example;

import com.example.utils.Ball;
import com.example.utils.Grid;
import com.example.utils.ImmutableVector;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.List;

public class Main extends JPanel {
    public static final int FPS = 60;

    private int frames = 0;
    private long lastFpsTime = System.nanoTime();
    private int fps = 0;

    private Simulation sim;
    private Rectangle2D ballBounds = new Rectangle2D.Double(100, 100, 800, 600);

    public static void main(String[] args){
        JFrame frame = new JFrame("Ball Collision Simulator");
        JPanel panel = new Main();

        frame.add(panel);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    Main(){
        setBackground(Color.BLACK);
        setPreferredSize(new Dimension(1000, 800));

        List<Ball> balls = spawnBalls(2, 40);

        Grid<List<Ball>> grid = new Grid<>(20, 20);
        for(int r = 0; r < grid.rows(); r++){
            for(int c = 0; c < grid.cols(); c++){
                grid.set(r, c, new ArrayList<>());
            }
        }

        sim = new SimulationSeq(ballBounds, balls, grid);

        new Thread(this::run).start();
    }

    private void run(){
        double frameTime = 1_000_000_000.0 / FPS;
        double delta = 0;
        long last = System.nanoTime();
        while(true){
            long now = System.nanoTime();
            delta += now - last;
            last = now;
            if(delta >= frameTime){
                delta -= frameTime;
                sim.update();
                repaint();

                frames++;
                if(now - lastFpsTime >= 1_000_000_000){
                    fps = frames;
                    frames = 0;
                    lastFpsTime = now;
                }
            }
        }
    }

    private List<Ball> spawnBalls(int n, double radius){
        List<Ball> balls = new ArrayList<>();
        for(int i = 0; i < n; i++){
            double x = ballBounds.getX() + Math.random() * ballBounds.getWidth();
            double y = ballBounds.getY() + Math.random() * ballBounds.getHeight();

            Ball b = new Ball(x, y, radius, 1);
            b.velocity = new ImmutableVector(Math.random()*100, Math.random()*100);
            balls.add(b);
        }
        return balls;
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;

        g2.setColor(Color.WHITE);
        g2.drawString("FPS: " + fps, 10, 20);

        g2.setColor(Color.WHITE);
        g2.draw(ballBounds);

        drawBalls(g2, sim.balls());

//        g2.setColor(Color.BLUE);
//        drawTrajectory(g2, sim.balls());
//
//        g2.setColor(Color.WHITE);
//        drawCoordinates(g2, sim.balls());
    }

    private void drawBalls(Graphics2D g2, List<Ball> balls){
        for(Ball ball : balls){
            int x = (int) ball.position.x;
            int y = (int) ball.position.y;
            int r = (int) ball.radius;
            g2.setColor(color(ball));
            g2.fillOval(x-r, y-r, r*2, r*2);
        }
    }

    private static void drawCoordinates(Graphics2D g2, List<Ball> balls){
        for(Ball ball : balls){
            int x = (int) ball.position.x;
            int y = (int) ball.position.y;
            int r = (int) ball.radius;
            int vx = (int) ball.velocity.x;
            int vy = (int) ball.velocity.y;

            g2.drawString("(%d, %d)".formatted(x, y), x+r, y);
            g2.drawString("(%d, %d)".formatted(vx, vy), x+r, y+20);
        }
    }

    private static void drawTrajectory(Graphics2D g2, List<Ball> balls){
        for(Ball ball : balls){
            int x = (int) ball.position.x;
            int y = (int) ball.position.y;

            g2.drawLine(x, y, (int) (x + ball.velocity.x/5), (int) (y + ball.velocity.y/5));
        }
    }

    private Color color(Ball ball){
        return switch(ball.id % 5){
            case 0 -> Color.WHITE;
            case 1 -> Color.RED;
            case 2 -> Color.GREEN;
            case 3 -> Color.CYAN;
            case 4 -> Color.YELLOW;
            default -> throw new IllegalStateException("Unexpected value: " + ball.id);
        };
    }
}
```

# SimulationSeq.java

```
package com.example;

import com.example.utils.Ball;
import com.example.utils.Grid;
import com.example.utils.Vector;

import java.awt.geom.Rectangle2D;
import java.util.List;

public class SimulationSeq implements Simulation {
    double gravity = 1000;
    Rectangle2D bounds;
    List<Ball> balls;
    Grid<List<Ball>> grid;

    public SimulationSeq(Rectangle2D bounds, List<Ball> balls, Grid<List<Ball>> grid) {
        this.bounds = bounds;
        this.balls = balls;
        this.grid = grid;
    }

    @Override
    public void update() {
        for(Ball b : balls){
            b.velocity.y += gravity/Main.FPS;
            b.position = b.position.add(Vector.div(b.velocity, Main.FPS));
        }
        for(Ball b : balls){
            resolveWallCollision(b);
        }
        for(int i = 0; i < balls.size(); i++){
            Ball a = balls.get(i);
            for(int j = i+1; j < balls.size(); j++){
                Ball b = balls.get(j);
                if(a.overlaps(b)){
                    resolveOverlap(a, b);
                    resolveCollision(a, b);
                }
            }
        }
    }

    void resolveWallCollision(Ball b){
        // Left wall
        if (b.position.x - b.radius < bounds.getX()){
            b.position.x = bounds.getX() + b.radius;
            b.velocity.x = -b.velocity.x;
        }

        // Right wall
        if (b.position.x + b.radius > bounds.getMaxX()){
            b.position.x = bounds.getMaxX() - b.radius;
            b.velocity.x = -b.velocity.x;
        }

        // Bottom wall
        if (b.position.y - b.radius < bounds.getY()){
            b.position.y = bounds.getY() + b.radius;
            b.velocity.y = -b.velocity.y;
        }

        // Top wall
        if (b.position.y + b.radius > bounds.getMaxY()){
            b.position.y = bounds.getMaxY() - b.radius;
            b.velocity.y = -b.velocity.y;
        }
    }

    void resolveCollision(Ball a, Ball b){
        Vector AB = a.position.sub(b.position);

        if(AB.isZero()){
            return;
        }
        Vector vAB = a.velocity.sub(b.velocity);

        double AB_scalar = 2/(a.mass+b.mass) * b.mass * vAB.dot(AB) / AB.dot(AB);
        AB = AB.scale(AB_scalar);

        a.velocity = a.velocity.sub(AB);
        b.velocity = b.velocity.add(AB);
    }

    void resolveOverlap(Ball a, Ball b) {
        Vector delta = a.position.sub(b.position);
        double distSq = delta.dot(delta);

        if (distSq == 0) {
            return;
        }

        double dist = Math.sqrt(distSq);
        double overlap = a.radius + b.radius - dist;

        if (overlap <= 0) {
            return;
        }

        Vector normal = delta.div(dist);

        double totalMass = a.mass + b.mass;

        double moveA = overlap * b.mass / totalMass;
        double moveB = overlap * a.mass / totalMass;

        a.position = a.position.add(normal.scale(moveA));
        b.position = b.position.sub(normal.scale(moveB));
    }

    @Override
    public List<Ball> balls(){
        return balls;
    }
}
```

```
package com.example.utils;

import com.example.Main;

public class Ball{
    private static int ID;
    public final int id = ID++;
    public Vector position, velocity;
    public final double radius, mass, elasticity;

    public Ball(double x, double y, double radius, double elasticity) {
        this.position = new ImmutableVector(x, y);
        this.velocity = new ImmutableVector(0, 0);
        this.radius = radius;
        this.mass = radius * radius;
        this.elasticity = elasticity;
    }

    public void update() {
        position = position.add(new Vector(velocity).scale(1.0/ Main.FPS));
    }

    public boolean overlaps(Ball other){
        double dx = position.x - other.position.x;
        double dy = position.y - other.position.y;
        double rSum = radius + other.radius;
        return dx*dx + dy*dy <= rSum*rSum;
    }
}
```

# ImmutableVector.java

```
package com.example.utils;

public class ImmutableVector extends Vector {
    public ImmutableVector(double x, double y) {
        super(x, y);
    }

    public ImmutableVector(Vector vector) {
        super(vector);
    }

    public ImmutableVector set(double x, double y) {
        return new ImmutableVector(x, y);
    }

    public static ImmutableVector add(Vector a, Vector b) {
        return new ImmutableVector(a.x + b.x, a.y + b.y);
    }

    @Override
    public ImmutableVector add(double dx, double dy) {
        return new ImmutableVector(x + dx, y + dy);
    }

    @Override
    public ImmutableVector add(Vector vector) {
        return new ImmutableVector(x + vector.x, y + vector.y);
    }

    public static ImmutableVector sub(Vector a, Vector b) {
        return new ImmutableVector(a.x - b.x, a.y - b.y);
    }

    @Override
    public ImmutableVector sub(double x, double y) {
        return new ImmutableVector(this.x - x, this.y - y);
    }

    @Override
    public ImmutableVector sub(Vector vector) {
        return new ImmutableVector(this.x - vector.x, this.y - vector.y);
    }

    public static ImmutableVector scale(Vector vector, double scalar) {
        return new ImmutableVector(vector.x * scalar, vector.y * scalar);
    }

    @Override
    public ImmutableVector scale(double scalar) {
        return new ImmutableVector(this.x * scalar, this.y * scalar);
    }

    public static ImmutableVector div(Vector vector, double divisor) {
        return new ImmutableVector(vector.x / divisor, vector.y / divisor);
    }

    @Override
    public ImmutableVector div(double divisor) {
        return new ImmutableVector(this.x / divisor, this.y / divisor);
    }

    @Override
    public ImmutableVector nor(){
        double len = Math.sqrt(x*x + y*y);
        return new ImmutableVector(x / len, y / len);
    }

    @Override
    public ImmutableVector reverse(){
        return new ImmutableVector(-this.x, -this.y);
    }

    @Override
    public double len() {
        return Math.sqrt(x*x + y*y);
    }

    @Override
    public double len2() {
        return x*x + y*y;
    }

    @Override
    public double distance(Vector vector) {
        double dx = vector.x - x;
        double dy = vector.y - y;
        return Math.sqrt(dx*dx + dy*dy);
    }

    @Override
    public double distance2(Vector vector) {
        double dx = vector.x - x;
        double dy = vector.y - y;
        return dx*dx + dy*dy;
    }

    /**
     * @return A positive number if the angle between {@code this} and the argument is less than 90 degrees. <br>
     * A negative number if the angle between {@code this} and the argument is more than 90 degrees. <br>
     * 0 if the angle between {@code this} and the argument is exactly 90 degrees. <br>
     */
    public double dot(double x, double y) {
        return this.x*x + this.y*y;
    }

    /**
     * @return A positive number if the angle between {@code this} and the argument is less than 90 degrees. <br>
     * A negative number if the angle between {@code this} and the argument is more than 90 degrees. <br>
     * 0 if the angle between {@code this} and the argument is exactly 90 degrees. <br>
     */
    public double dot(ImmutableVector vector) {
        return x*vector.x + y*vector.y;
    }
}
```
