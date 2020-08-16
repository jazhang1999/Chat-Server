import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.net.*;
import java.io.*;
import java.awt.*;

public class JavaGUI extends JFrame {
    private Socket socket              = null;
    // private BufferedReader streamIn  = null;
    private BufferedWriter streamOut = null;
    private String serverName = "localhost";
    private int    serverPort = 8080;
    private ChatClientThread client = null;

    // GUI components
    private JPanel mainPanel;
    private JButton sendButton;
    private JButton connectButton;
    private JTextArea msgDisplay;
    private JFormattedTextField userTextField;

    public JavaGUI(String title) {

        // Set up window for use
        super(title);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setContentPane(mainPanel);
        this.pack();
        msgDisplay.setEditable(false);
        sendButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg_out = userTextField.getText();
                userTextField.setText("");
                send(msg_out);
            }
        });

        /* Connects to the server, create the buffers, and begin running the Chat Client
         * Thread to handle incoming messages from other users / the server */
        connectButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                connect(serverName, serverPort);
                start();
            }
        });
    }

    public void connect(String serverName, int serverPort){
        msgDisplay.append("Attempting to establish the connection, please wait ..." + "\n");
        try {
            socket = new Socket(serverName, serverPort);
            msgDisplay.append("Connected: " + socket + "\n");
            connectButton.setEnabled(false);
            client = new ChatClientThread(this, socket);
        } catch(UnknownHostException uhe) {
            msgDisplay.append("Host unknown: " + uhe.getMessage() + "\n");
        } catch(IOException ioe) {
            msgDisplay.append("Unexpected exception: " + ioe.getMessage() + "\n");
        }
    }

    // Sets up the streams to be able to accept server input and return client output
    public void start() {
        try {
            // streamIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            streamOut = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        } catch (IOException ioe) {
            msgDisplay.append("Connecting to streams failed: " + ioe.getMessage());
        }
    }

    // Sends the information to a public source
    public void send(String msg_out) {
        try {
            boolean leave = false;
            if (msg_out.equals("Exit")) {
                msg_out = "Server: This user has exited the chat";
                leave = true;
            }
            streamOut.write(msg_out);
            streamOut.flush();
            System.out.println("Client has written to some source");
            if (leave) {
                close();
            }
        } catch(IOException ioe) {
            System.out.println("Sending error: " + ioe.getMessage());
        }
    }
    public void handle(String msg) {
        if (msg.equals("")) {
            msgDisplay.append("Good bye. Press RETURN to exit ...");
            close();
        } else
            msgDisplay.append(msg + "\n");
    }

    public void close() {
        try {
            if (streamOut != null)
                streamOut.close();
            if (socket != null)
                socket.close();
            System.exit(0);
        } catch (IOException ioe) {
            msgDisplay.append("Error closing: " + ioe.getMessage());
        }
    }

    public static void main(String[] args) {
        JFrame frame = new JavaGUI("My Chat Application");
        frame.setVisible(true);
    }
}
