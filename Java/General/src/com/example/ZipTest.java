package com.example;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class ZipTest {
    private File createZipFile(File[] files) throws IOException {

       // Random random = new Random();
        File zipFile = new File("D:\\zip.zip");

        FileOutputStream fileOutputStream = new FileOutputStream(zipFile);
        ZipOutputStream zipOutputStream = new ZipOutputStream(new BufferedOutputStream(fileOutputStream));

        for (File file: files) {
            BufferedInputStream bufferedInputStream = null;
            byte data[] = new byte[1024];
            FileInputStream fileInputStream = new FileInputStream(file.getAbsolutePath());
            bufferedInputStream = new BufferedInputStream(fileInputStream, 1024);

            ZipEntry entry = new ZipEntry(file.getName());
            zipOutputStream.putNextEntry(entry);
            int count;
            while ((count = bufferedInputStream.read(data, 0, 1024)) != -1) {
                zipOutputStream.write(data, 0, count);
            }
            bufferedInputStream.close();
            fileInputStream.close();
        }

        zipOutputStream.close();

        return zipFile;
    }
    
    public static void main(String[] args) {
        new ZipTest().run();
    }

    private void run() {
        try {
            createZipFile(new File[] {new File("c:\\temp\\test1.txt"), new File("c:\\temp\\test2.txt")} );
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
