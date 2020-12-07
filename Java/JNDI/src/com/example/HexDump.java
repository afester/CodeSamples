package com.example;

public class HexDump {
   
   private int addr;
   private StringBuffer hexData;
   private StringBuffer charData;
   private static final int BYTES_PER_LINE = 16;
   private int offset;
   private String prefix = "";
   
   public HexDump() {
      addr = 0;
      offset = 0;
      hexData = new StringBuffer(3 * BYTES_PER_LINE);
      charData = new StringBuffer(BYTES_PER_LINE);
   }


   public void setPrefix(String aPrefix) {
      prefix = aPrefix;
   }

   public void dump(byte[] data) {
      // process the data array
      for (int idx = 0;  idx < data.length;  idx++) {

         // get the current value
         int value = (int) data[idx] & 0xFF;
   
         // format value as hex string
         String hex = String.format(" %02X", value);
   
         // format value as (extended) ascii character
         if (value < 32) {
            value = '.';
         }
         String ascii = String.format("%c", value);
         
         // append to result strings
         hexData.append(hex);
         charData.append(ascii);

         offset++;
         if (offset == 16) {
            emitLine();
         }
      }
   }


   public void done() {
      if (offset > 0) {
         while(offset < 16) {
            hexData.append("   ");
            offset++;
         }
         emitLine();
      }
   }


   void emitLine() {
      System.err.println(String.format("%s%08x:%s %s", prefix, addr, hexData, charData));

      // reset state
      offset = 0;
      addr += BYTES_PER_LINE;
      hexData.setLength(0);
      charData.setLength(0);
   }
   
   
   public static void main(String[] args) {
      byte[] data = new byte[256];
      for (int b = 0;  b < data.length;  b++) {
         data[b] = (byte) (b & 0xff);
      }

      HexDump dump = new HexDump();
      dump.dump(data);
      dump.done();
   }
}