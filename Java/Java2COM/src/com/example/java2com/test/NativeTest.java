package com.example.java2com.test;

import static org.junit.Assert.*;

import java.io.File;

import org.junit.Test;

import com.example.java2com.COMProxy;
import com.example.java2com.Variant;


public class NativeTest {

    @Test
    public void testGetPropertyExcel() {
        COMProxy excel = new COMProxy("Excel.Application");

        Variant result = excel.getProperty("Name");
        assertEquals(Variant.VT_BSTR, result.vt);
        assertEquals("Microsoft Excel", result.strValue);

        excel.release();
    }

    
    @Test
    public void testGetPropertyWord() {
        COMProxy word = new COMProxy("Word.Application");

        Variant result = word.getProperty("Options");
        COMProxy options = result.dispatch;
        
        result = options.getProperty("ConfirmConversions");
        assertEquals(Variant.VT_BOOL, result.vt);
        assertEquals(false, result.booleanValue);

        result = word.getProperty("FeatureInstall");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(0, result.intValue);

        result = word.getProperty("Name");
        assertEquals(Variant.VT_BSTR, result.vt);
        assertEquals("Microsoft Word", result.strValue);

        // Required for word to be terminated! Excel does not require this, though ...
        word.invoke("Quit");

        word.release();
    }

    
    @Test
    public void testInvoke() {
        COMProxy excel = new COMProxy("Excel.Application");

        Variant result = excel.getProperty("Workbooks");
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);

        COMProxy workbooks = result.dispatch;
        result = workbooks.invoke("Add");
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        workbooks.release();

        // Retrieve the sheets collection in the Workbook
        COMProxy workbook = result.dispatch;
        result = workbook.getProperty("Sheets");
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        workbook.release();

        // Retrieve the number of sheets in the workbook
        COMProxy sheets = result.dispatch;
        result = sheets.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(3, result.intValue);
        sheets.release();

        excel.release();
    }
    
    @Test
    public void testAddDocument() {
        COMProxy word = new COMProxy("Word.Application");

        // word.setProperty("Visible", new Variant(true));

        Variant result = word.getProperty("Documents");
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        COMProxy documents = result.dispatch;

        // check the number of documents in the collection - should be 0
        result = documents.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(0, result.intValue);

        // Add a new empty document
        documents.invoke("Add");

        // check the number of documents in the collection - should be 1
        result = documents.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(1, result.intValue);

/*
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
*/
        word.invoke("Quit");
        word.release();
    }

    @Test
    public void testLoadDocument() {
        COMProxy word = new COMProxy("Word.Application");

        // make the application visible
        // word.setProperty("Visible", new Variant(true));

        // get the Documents collection
        Variant result = word.getProperty("Documents");
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        COMProxy documents = result.dispatch;

        // check the number of documents in the collection - should be 0
        result = documents.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(0, result.intValue);

        // Open the test document
        String projDir = System.getProperty("user.dir");
        String fileName = projDir + File.separator + "Properties.docx";
        result = documents.invoke("Open", new Variant(fileName));
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        COMProxy document = result.dispatch;

        // check the number of documents in the collection - should be 1
        result = documents.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(1, result.intValue);

        // Close the document
        document.invoke("Close");

        // Quit the application
        word.invoke("Quit");

        word.release();
    }

    @Test
    public void testListProperties() {
        COMProxy word = new COMProxy("Word.Application");

        // get the Documents collection
        Variant result = word.getProperty("Documents");
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        COMProxy documents = result.dispatch;

        // check the number of documents in the collection - should be 0
        result = documents.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(0, result.intValue);

        // Open the test document
        String projDir = System.getProperty("user.dir");
        String fileName = projDir + File.separator + "Properties.docx";
        result = documents.invoke("Open", new Variant(fileName));
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        COMProxy document = result.dispatch;

        // check the number of documents in the collection - should be 1
        result = documents.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(1, result.intValue);

        // retrieve the built in properties
        //result = document.getProperty("BuiltInDocumentProperties");
        result = document.getProperty("CustomDocumentProperties");
        assertEquals(Variant.VT_DISPATCH, result.vt);
        assertNotNull(result.dispatch);
        COMProxy propertyCollection = result.dispatch;

        // retrieve the number of properties
        result = propertyCollection.getProperty("Count");
        assertEquals(Variant.VT_I4, result.vt);
        assertEquals(2, result.intValue);

        String[] expectedProperties = {"Source=Unknown", 
                                       "Reference=Somewhere"};

        // get the properties
        int propertyCount = result.intValue;
        for (int idx = 1;  idx <= propertyCount;  idx++) {
            // read the DocumentProperty at the given index
            result = propertyCollection.getProperty("Item", new Variant(idx));
            assertEquals(Variant.VT_DISPATCH, result.vt);
            assertNotNull(result.dispatch);
            COMProxy documentProperty = result.dispatch;
    
            // get the name of the property
            result = documentProperty.getProperty("Name");
            assertEquals(Variant.VT_BSTR, result.vt);
            assertNotNull(result.strValue);
            String name = result.strValue;
            
            // get the value of the property
            result = documentProperty.getProperty("Value");
            assertEquals(Variant.VT_BSTR, result.vt);
            assertNotNull(result.strValue);
            String value = result.strValue;

            // check if the property has been properly returned
            String property = name + "=" + value;
            assertEquals(expectedProperties[idx - 1], property);
        }

        // Close the document
        document.invoke("Close");

        // Quit the application
        word.invoke("Quit");

        word.release();
    }

}
