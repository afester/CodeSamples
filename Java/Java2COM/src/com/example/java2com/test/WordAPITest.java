package com.example.java2com.test;

import static org.junit.Assert.assertEquals;

import java.io.File;

import org.junit.Before;
import org.junit.Test;

import com.example.java2com.msword.Document;
import com.example.java2com.msword.Documents;
import com.example.java2com.msword.WordApplication;


public class WordAPITest {

    private String propertyTestFile;
    
    @Before
    public void setupFilenames() {
        String projDir = System.getProperty("user.dir");
        propertyTestFile = projDir + File.separator + "Properties.docx";
    }


    @Test
    public void testWordAPI() {
        WordApplication appl = new WordApplication();
        assertEquals("Microsoft Word", appl.getName());

        Documents docs = appl.getDocuments();
        assertEquals(0, docs.getCount());
        docs.release();

        Document d = docs.open(propertyTestFile);

        docs = appl.getDocuments();
        assertEquals(1, docs.getCount());
        docs.release();

        assertEquals("Properties.docx", d.getName());

        d.release();
        appl.release();
    }

}
