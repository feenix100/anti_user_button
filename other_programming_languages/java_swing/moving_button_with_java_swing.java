import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;

public class MovingButtonApp extends JFrame {
    private JButton movingButton;
    private int speed = 10;
    private int mouseNearThreshold = 100;
    private Random random;

    public MovingButtonApp() {
        setTitle("Moving Button");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(null);

        random = new Random();
        
        movingButton = new JButton("<html>Click here to keep<br>an idiot busy for hours!</html>");
        movingButton.setBounds(400, 300, 200, 50);
        movingButton.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                moveButtonRandomly();
            }
        });
        
        add(movingButton);
        setVisible(true);
    }

    private void moveButtonRandomly() {
        int buttonX = movingButton.getX();
        int buttonY = movingButton.getY();
        
        int dx = random.nextInt(speed * 2) - speed;
        int dy = random.nextInt(speed * 2) - speed;
        
        int newX = Math.max(0, Math.min(getWidth() - movingButton.getWidth(), buttonX + dx));
        int newY = Math.max(0, Math.min(getHeight() - movingButton.getHeight(), buttonY + dy));
        
        movingButton.setLocation(newX, newY);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new MovingButtonApp());
    }
}
