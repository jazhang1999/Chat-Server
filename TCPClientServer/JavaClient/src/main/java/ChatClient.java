/*
Code from http://pirate.shu.edu/~wachsmut/Teaching/CSAS2214/Virtual/Lectures/chat-client-server.html
DataInputStream/DataOutputStream has been deprecated and many of their methods do not properly handle
data sent between sockets / each other. I have taken the liberty of switching it out with BufferedReader
and BufferedWriter, and respective methods. Built on JDK 1.14
 */
import java.net.*;
import java.io.*;
import java.nio.Buffer;

public class ChatClient implements Runnable
{  private Socket socket              = null;
    private Thread thread              = null;
    private BufferedReader  console   = null;
    private BufferedWriter  streamOut = null;
    private ChatClientThread client    = null;

    public ChatClient(String serverName, int serverPort) {
        System.out.println("Establishing connection. Please wait ...");
        try {
            socket = new Socket(serverName, serverPort);
            System.out.println("Connected: " + socket);
            start();
        } catch(UnknownHostException uhe) {
            System.out.println("Host unknown: " + uhe.getMessage());
        } catch(IOException ioe) {
            System.out.println("Unexpected exception: " + ioe.getMessage());
        }
    }

    public void start() throws IOException {
        console   = new BufferedReader(new InputStreamReader(System.in));
        streamOut = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        if (thread == null) {
            client = new ChatClientThread(this, socket);
            thread = new Thread(this);
            thread.start();
        }
        System.out.println("Finished starting up");
    }

    public void run() {
        while (thread != null) {
            try {
                streamOut.write(console.readLine());
                streamOut.flush();
                System.out.println("Console has written to some source");
            } catch(IOException ioe) {
                System.out.println("Sending error: " + ioe.getMessage());
                stop();
            }
        }
    }

    public void handle(String msg) {
        System.out.println("Got to handling stage");
        if (msg.equals("")) {
            System.out.println("Good bye. Press RETURN to exit ...");
            stop();
        }
        else
            System.out.println(msg);
    }

    public void stop() {
        System.out.println("Reached the stop stage");
        if (thread != null) {
            thread.interrupt();
            thread = null;
        }
        try {
            if (console   != null) {
                console.close();
                System.out.println("Finished closing");
            } if (streamOut != null) {
                streamOut.close();
                System.out.println("Finished closing");
            } if (socket    != null) {
                socket.close();
                System.out.println("Finished closing");
            }
        } catch(IOException ioe) {
            System.out.println("Error closing ..."); }
        client.close();
        client.interrupt();
    }

    public static void main(String args[])
    {  ChatClient client = null;
        if (args.length != 2) {
            System.out.println("Usage: java ChatClient host port");
            client = new ChatClient("localhost", 8080);
        } else
            client = new ChatClient(args[0], Integer.parseInt(args[1]));
    }
}