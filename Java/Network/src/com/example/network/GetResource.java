package com.example.network;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;

import com.example.hexdump.HexDump;

public class GetResource {

    private final int DEST_PORT;
    private InetAddress DEST_IP;
    private final String RESOURCE;

    public GetResource(String ipAddr, String port, String url) {
        try {
            DEST_IP = InetAddress.getByName(ipAddr);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        DEST_PORT = Integer.parseInt(port);
        RESOURCE = url;
    }


    private void run() {
        Socket client = new Socket();
        SocketAddress dest = new InetSocketAddress(DEST_IP, DEST_PORT);
        try {
            System.err.println("DEST=" + dest);
            client.connect(dest);

            System.err.println("Connected.\n");

            OutputStream out = client.getOutputStream();
            InputStream in = client.getInputStream();

            String request = String.format(
                    "GET %s HTTP/1.0\n\n"   // Most simple GET request for HTTP/1.0 - HTTP/1.1 requires the Host header!
                    ,  RESOURCE, DEST_IP.getHostAddress());
            System.err.println("Request --------------------------------------------------------");
            System.err.println(request);
            out.write(request.getBytes());

            System.err.println("Response -------------------------------------------------------");
            BufferedReader br = new BufferedReader(new InputStreamReader(in));
            String line = null;
            while( (line = br.readLine()) != null) {
                System.err.println(line);
            }
            System.err.println("----------------------------------------------------------------");

            // byte[] buffer = new byte[1024];
            //int data = in.read(buffer);
            //System.err.println(data);
            //HexDump hd = new HexDump(buffer);
            //hd.dumpAll(System.err);
            //client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static void main(String[] args) {
        if (args.length != 3) {
            System.err.println("Usage: GetResource IpAddress Port URL\n");
        } else {
            GetResource app = new GetResource(args[0], args[1], args[2]);
            app.run();
        }
    }
}
