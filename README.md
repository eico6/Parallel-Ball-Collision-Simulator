# Parallel-Bouncing-Balls

## Development notes
- C++ version: g++.exe (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders, r8) 13.2.0
- Graphics library: raylib version 5.5


## TEMP

Java code:

```
package com.example;

import com.example.utils.Ball;
import com.example.utils.Grid;

import javax.swing.*;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.util.ArrayList;
import java.util.List;

public class Main extends JPanel implements Runnable {
    public static final int FPS = 60;

    private Thread thread;
    private Simulation sim;

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
        List<Ball> balls = new ArrayList<>();
        balls.add(new Ball(100, 100, 20, 20));

        Grid<List<Ball>> grid = new Grid<>(20, 20);

        setPreferredSize(new Dimension(800, 600));
        setBackground(Color.BLACK);
        sim = new SimulationSeq(getBounds(), balls, grid);

        thread = new Thread(this);
        thread.start();
    }

    @Override
    public void run(){
        double frameTime = 1000000000.0 / FPS;
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
            }
        }
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;

        g2.setColor(Color.WHITE);
        g2.draw(getBounds());
        drawBalls(g2, sim.balls());
    }

    private void drawBalls(Graphics2D g2, List<Ball> balls){
        for(Ball ball : balls){
            int x = (int) ball.position.x;
            int y = (int) ball.position.y;
            int r = (int) ball.radius;

            g2.fillOval(x, y, r, r);
        }
    }
}
```
