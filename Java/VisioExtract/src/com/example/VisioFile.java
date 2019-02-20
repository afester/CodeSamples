package com.example;

import java.io.FileInputStream;
import java.io.IOException;

import org.apache.poi.hdgf.HDGFDiagram;
import org.apache.poi.hdgf.chunks.Chunk;
import org.apache.poi.hdgf.streams.ChunkStream;
import org.apache.poi.hdgf.streams.PointerContainingStream;
import org.apache.poi.hdgf.streams.Stream;
import org.apache.poi.hdgf.streams.StringsStream;
import org.apache.poi.hpsf.DocumentSummaryInformation;
import org.apache.poi.hpsf.SummaryInformation;
import org.apache.poi.poifs.filesystem.DirectoryNode;
import org.apache.poi.poifs.filesystem.Entry;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

import com.example.hexdump.HexDump;

// PointerContainingStream:
//          type=30 => shape types used in the diagram
//          type=39 => 
public class VisioFile {

    private String fileName;

    public VisioFile(String name) {
        fileName = name;
    }

    
    public String getName() {
        return fileName;
    }

    private void dumpStream(int level, Stream stream) {
        String indent = "                                                     ".substring(0, level*3);

        if (level == 0) System.err.println("\nTOP LEVEL");
        if(stream instanceof StringsStream) {
            System.err.println(indent + "Strings stream");
            StringsStream ss = (StringsStream)stream;

            HexDump hd = new HexDump(ss._getStore()._getContents());
            hd.setPrefix(indent);
            hd.dumpAll(System.err);

        } else if(stream instanceof PointerContainingStream) {
            PointerContainingStream ps = (PointerContainingStream)stream;

            System.err.printf("%sPointerContainingStream: addr=0x%08x, ofs=%04x, len=%d, type=%d, size=%d\n", 
                              indent,
                              ps.getPointer().getAddress(),
                              ps.getPointer().getOffset(),
                              ps.getPointer().getLength(),
                              ps.getPointer().getType(),
                              ps.getPointer().getSizeInBytes()
                              );
            
            dumpStreams(level + 1, ps.getPointedToStreams());

        } else if(stream instanceof ChunkStream) {
            ChunkStream cs = (ChunkStream)stream;
            System.err.println(indent + "Chunk stream " + cs.getPointer().getType());

            for (Chunk chunk : cs.getChunks()) {
                System.err.println("   " + indent + "CHUNK:" + chunk.getName() + "/" + chunk.getHeader().getType());
                
                HexDump hd = new HexDump(chunk._getContents());
                hd.setPrefix(indent + "   ");
                
                hd.dumpAll(System.err);
            }

        } else {
            System.err.println(indent + "Unknown stream (" + stream._getContentsLength() + " bytes)");

            HexDump hd = new HexDump(stream._getStore()._getContents());
            hd.setPrefix(indent);
//            hd.dumpAll(System.err);
        }

    }
    private void dumpStreams(int level, Stream[] streams) {

        for (Stream stream : streams) {
            dumpStream(level, stream);
        }

    }

    public void extractComponents() {
/*        
        byte[] buffer = new byte[65536];
        try {
            RandomAccessFile raf = new RandomAccessFile(fileName, "r");
            raf.read(buffer,  0,  buffer.length);
            HexDump hd = new HexDump(buffer);
            hd.dumpAll(System.err);;
        } catch (IOException e1) {
            e1.printStackTrace();
        }
*/

        try {
            POIFSFileSystem fs = new POIFSFileSystem(new FileInputStream(fileName));
            DirectoryNode root = fs.getRoot();
            for (Entry e : root) {
                System.err.printf("  %s (%s)\n", e.getName(),
                                  (e.isDirectoryEntry() ? "DIR" : "") +
                                  (e.isDocumentEntry() ? "DOC" : ""));
            }
            
            HDGFDiagram diagram = new HDGFDiagram(fs);

            System.err.println("\nSummary information:");
            System.err.println("--------------------------------");
            SummaryInformation summary = diagram.getSummaryInformation();
            System.err.printf("  Application Name: %s\n", summary.getApplicationName());
            System.err.printf("  Author: %s\n", summary.getAuthor());
            System.err.printf("  Comments: %s\n", summary.getComments());
            System.err.printf("  Creation date: %s\n", summary.getCreateDateTime());
            System.err.printf("  Edit date: %s\n", summary.getEditTime());
            System.err.printf("  Keywords: %s\n", summary.getKeywords());
            System.err.printf("  Last Author: %s\n", summary.getLastAuthor());
            System.err.printf("  Last Printed: %s\n", summary.getLastPrinted());
            System.err.printf("  Last Saved: %s\n", summary.getLastSaveDateTime());
            System.err.printf("  Page count: %s\n", summary.getPageCount());
            System.err.printf("  Section count: %s\n", summary.getSectionCount());
            System.err.printf("  Subject: %s\n", summary.getSubject());
            System.err.printf("  Template: %s\n", summary.getTemplate());
            System.err.printf("  Title: %s\n", summary.getTitle());
            System.err.printf("  Word cound: %s\n", summary.getWordCount());

            System.err.println("\nDocument Summary information:");
            System.err.println("--------------------------------");
            DocumentSummaryInformation docSummary = diagram.getDocumentSummaryInformation();
            System.err.printf("  Category: %s\n", docSummary.getCategory());
            System.err.printf("  Company: %s\n", docSummary.getCompany());
            System.err.printf("  Note count: %s\n", docSummary.getNoteCount());
            System.err.printf("  OS Version: %s\n", docSummary.getOSVersion());
            System.err.printf("  Par count: %s\n", docSummary.getParCount());
            System.err.printf("  Presentation format: %s\n", docSummary.getPresentationFormat());
            System.err.printf("  Section count: %s\n", docSummary.getSectionCount());
            System.err.printf("  Slide count: %s\n", docSummary.getSlideCount());

            // Seems that the Trailer stream is only a pointer to a collection of all top level streams ...
            //System.err.println("#################### TRAILER");
            //dumpStream(0, diagram.getTrailerStream());
            //System.err.println("#################### TOP LEVEL");

            dumpStreams(0, diagram.getTopLevelStreams());
        } catch (IOException e1) {
            e1.printStackTrace();
        }

/*
        File f = new File(fileName);
        System.err.println(f.exists());

        try {
            ZipFile file = new ZipFile(fileName);
            Enumeration<? extends ZipEntry> entries = file.entries();
            file.stream().forEach(entry -> {
                System.err.println("x=" + entry);
            });
        } catch (IOException e) {
            e.printStackTrace();
        }*/
    }
}
