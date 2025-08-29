import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class MovingButtonApp extends JFrame {
    // Tunables
    private static final int WINDOW_W = 800, WINDOW_H = 600;
    private static final int BTN_W = 200, BTN_H = 50;
    private static final int TICK_MS = 16;           // ~60 FPS
    private static final float REPEL_RADIUS = 120f;  // repel if cursor is within this distance of button center
    private static final float REPEL_STEP = 14f;     // pixels per tick when repelling
    private static final float RETURN_SPEED = 6f;    // pixels per tick moving back to center
    private static final float CENTER_DEADZONE = 2f; // stop moving if <= this many pixels from true center

    private final JButton movingButton =
            new JButton("<html>Click here to keep<br>an idiot busy for hours!</html>");
    private final Timer loop;

    public MovingButtonApp() {
        super("Moving Button");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        Container root = getContentPane();
        root.setLayout(null);
        root.setBackground(Color.WHITE);

        setSize(WINDOW_W, WINDOW_H);
        setLocationRelativeTo(null);

        // Start centered
        Point center = getClientCenter();
        movingButton.setBounds(center.x - BTN_W / 2, center.y - BTN_H / 2, BTN_W, BTN_H);
        root.add(movingButton);
        setVisible(true);

        // Animation loop
        loop = new Timer(TICK_MS, e -> tick());
        loop.start();
    }

    private void tick() {
        Container root = getContentPane();

        // Current button center
        float bx = movingButton.getX() + BTN_W * 0.5f;
        float by = movingButton.getY() + BTN_H * 0.5f;

        // Window/client center (target)
        Point c = getClientCenter();
        float cx = c.x, cy = c.y;

        // Distance from center
        float cdx = cx - bx, cdy = cy - by;
        float distToCenter = (float) Math.hypot(cdx, cdy);

        // Mouse position (null if outside window)
        Point mouse = root.getMousePosition();
        float dx = 0f, dy = 0f;

        if (mouse != null) {
            float mx = mouse.x, my = mouse.y;
            float vdx = bx - mx, vdy = by - my;
            float distToMouse = (float) Math.hypot(vdx, vdy);

            if (distToMouse < REPEL_RADIUS && distToMouse > 0f) {
                // Repel away from cursor
                dx = (vdx / distToMouse) * REPEL_STEP;
                dy = (vdy / distToMouse) * REPEL_STEP;
            } else if (distToCenter > CENTER_DEADZONE) {
                // Cursor not close; move toward center until inside deadzone
                dx = (cdx / distToCenter) * RETURN_SPEED;
                dy = (cdy / distToCenter) * RETURN_SPEED;
            } // else: inside deadzone and cursor not close -> do nothing (stop)
        } else {
            // Mouse not over window; only move toward center if outside deadzone
            if (distToCenter > CENTER_DEADZONE) {
                dx = (cdx / distToCenter) * RETURN_SPEED;
                dy = (cdy / distToCenter) * RETURN_SPEED;
            }
        }

        // Apply movement only if we actually need to move
        if (dx != 0f || dy != 0f) {
            float newCenterX = bx + dx;
            float newCenterY = by + dy;

            // Convert center back to top-left
            int newX = Math.round(newCenterX - BTN_W / 2f);
            int newY = Math.round(newCenterY - BTN_H / 2f);

            // Clamp to client area
            Dimension client = getClientSize();
            newX = clamp(newX, 0, Math.max(0, client.width - BTN_W));
            newY = clamp(newY, 0, Math.max(0, client.height - BTN_H));

            movingButton.setLocation(newX, newY);
        }
    }

    private Point getClientCenter() {
        Dimension d = getClientSize();
        return new Point(d.width / 2, d.height / 2);
    }

    private Dimension getClientSize() {
        return getContentPane().getSize();
    }

    private static int clamp(int v, int lo, int hi) {
        return Math.max(lo, Math.min(hi, v));
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(MovingButtonApp::new);
    }
}
