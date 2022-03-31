import javax.swing.*;
import java.awt.*;

public class Run extends JFrame {
    int numofgeneration = 200;
    Run() {
        JLabel label = new JLabel();
        JPanel panel = new JPanel();
        label.setDoubleBuffered(true);
        panel.add(label);
        panel.setDoubleBuffered(true);
        panel.setBackground(Color.black);
        add(panel);
        setVisible(true);
        setSize(5000,1000);
        for(int i =0;i<numofgeneration;i++) {
            label.setIcon(new ImageIcon(new GridGenerator("data/data" + i).getImage()));
        }
    }
    public static void main(String[] args) {
        new Run();
    }
}
