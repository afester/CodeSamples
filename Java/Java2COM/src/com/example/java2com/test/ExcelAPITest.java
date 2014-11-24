package com.example.java2com.test;

import static org.junit.Assert.assertEquals;

import java.io.File;

import org.junit.Before;
import org.junit.Test;

import com.example.java2com.IDispatch;
import com.example.java2com.msexcel.ExcelApplication;
import com.example.java2com.msexcel.Sheets;
import com.example.java2com.msexcel.Workbook;
import com.example.java2com.msexcel.Workbooks;
import com.example.java2com.msexcel.Worksheet;


public class ExcelAPITest {

    private String sampleFile;
    
    @Before
    public void setupFilenames() {
        String projDir = System.getProperty("user.dir");
        sampleFile = projDir + File.separator + "Sample.xls";
        IDispatch.setDebugEnabled(false);
    }


    @Test
    public void testExcelAPI() {
        ExcelApplication appl = new ExcelApplication();
        assertEquals("Microsoft Excel", appl.getName());

        Workbooks workbooks = appl.getWorkbooks();
        assertEquals(0, workbooks.getCount());

        Workbook sample = workbooks.open(sampleFile);
        assertEquals(1, workbooks.getCount());

        Sheets sheets = sample.getSheets();
        assertEquals(3, sheets.getCount());

        for (Worksheet sheet : sheets) {
            System.err.println(sheet.getName());
        }

        sheets.release();
        sample.release();
        workbooks.release();
        appl.release();
    }

}
