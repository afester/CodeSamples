package com.example.tree;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TreeReader {

    public static TreeNode<NodeData> loadTree(File selectedFile) {
        TreeNode<NodeData> result = null; 

        System.err.println("Loading " + selectedFile);
        
        BufferedReader bis;
        List<TreeNode<NodeData>> itemPath = new ArrayList<>();
        try {
            // String regexp = "(.+),\\s*(.+)\\s+(.+)";
            String regexp = "(\\+*)NodeData\\[label=(.+), order=(\\d+)(?:, additionalText=(.*))?\\]";
            Pattern pattern = Pattern.compile(regexp);

            bis = new BufferedReader(new FileReader(selectedFile));
            String line = null;
            while((line = bis.readLine()) != null) {
                Matcher matcher = pattern.matcher(line);
                matcher.find();

                // read next node
                int level = matcher.group(1).length();
                String label = matcher.group(2);
                int order = Integer.parseInt(matcher.group(3));
                String additionalText = "";
                if (matcher.groupCount() > 3) {
                    additionalText = matcher.group(4);
                }
                NodeData nodeData = new NodeData(label, order, additionalText);

                TreeNode<NodeData> node = new TreeNode<>(nodeData);
                System.err.println(level + " " + node);

                if (level == 0) {   // root node - only one root currently supported.
                    result = node;
                    itemPath.clear();
                    itemPath.add(result);
                } else {
                    TreeNode<NodeData> parent = itemPath.get(level -1);
                    parent.addChildren(node);

                    if (level >= itemPath.size()) {
                        itemPath.add(node);
                    } else {
                        itemPath.set(level, node);
                    }
                }

            }
        } catch (FileNotFoundException e1) {
            e1.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        return result;
    }
}
