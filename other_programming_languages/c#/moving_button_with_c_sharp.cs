using System;
using System.Drawing;
using System.Windows.Forms;

public class MovingButtonApp : Form
{
    private Button movingButton;
    private Timer timer;
    private Random random;
    private int speed = 10;
    private int mouseNearThreshold = 100;

    public MovingButtonApp()
    {
        this.Text = "Moving Button";
        this.Size = new Size(800, 600);
        
        random = new Random();
        
        movingButton = new Button();
        movingButton.Text = "Click here to \nunsubscribe!";
        movingButton.Size = new Size(200, 50);
        movingButton.Location = new Point(300, 300);
        movingButton.MouseMove += new MouseEventHandler(OnMouseMove);
        
        this.Controls.Add(movingButton);
    }

    private void OnMouseMove(object sender, MouseEventArgs e)
    {
        int mouseX = e.X;
        int mouseY = e.Y;
        int buttonX = movingButton.Location.X;
        int buttonY = movingButton.Location.Y;

        int dx = mouseX - buttonX;
        int dy = mouseY - buttonY;
        double dist = Math.Sqrt(dx * dx + dy * dy);

        if (dist < mouseNearThreshold)
        {
            int angle = random.Next(360);
            dx = (int)(speed * Math.Cos(angle));
            dy = (int)(speed * Math.Sin(angle));
        }
        else
        {
            int centerX = this.ClientSize.Width / 2;
            int centerY = this.ClientSize.Height / 2;
            dx = centerX - buttonX;
            dy = centerY - buttonY;
            double centerDist = Math.Sqrt(dx * dx + dy * dy);
            if (centerDist > 0)
            {
                dx = (int)((dx / centerDist) * speed);
                dy = (int)((dy / centerDist) * speed);
            }
        }

        int newX = Math.Max(0, Math.Min(this.ClientSize.Width - movingButton.Width, buttonX + dx));
        int newY = Math.Max(0, Math.Min(this.ClientSize.Height - movingButton.Height, buttonY + dy));

        movingButton.Location = new Point(newX, newY);
    }

    [STAThread]
    public static void Main()
    {
        Application.EnableVisualStyles();
        Application.Run(new MovingButtonApp());
    }
}
