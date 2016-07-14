package com.example.tree;

import java.util.HashMap;
import java.util.Map;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.scene.shape.Line;
import javafx.scene.text.Text;
import javafx.stage.Stage;

class LayoutData<T> {
    int yCoordinate;
    float xCoordinate;
    float flPrelim;
    float flModifier;  // mod
    
    TreeNode<T> prev;   // left neighbor
}


/* 
 * http://www.drdobbs.com/positioning-nodes-for-general-trees/184402320?pgno=4
 *
 *  - This algorithm operates in time O(N), where N is the number of nodes in the tree.
 *  - Two tree traversals are used to produce a node's final x-coordinate. 
 *    The first traversal assigns the preliminary x-coordinate and modifier fields for each node. 
 *    The second traversal computes the final x-coordinate of each node by summing the node's 
 *    preliminary x-coordinate with the modifier fields of all of its ancestors. 
 *    This technique makes moving a large subtree simple and allows the algorithm to operate in time 0(N).
 *  - The first tree traversal is a postorder traversal, positioning the smallest subtrees (the leaves) 
 *    first and recursively proceeding from left to right to build up the position of larger and larger subtrees.
 *  - The second tree traversal, a preorder traversal, determines the final x-coordinate for each node.
 */

public class TreeGrapher<T> extends Application {

    private static TreeNode staticNode;             // HACK!
    private TreeNode<T> rootNode = staticNode;      // HACK!

    Map<Integer, Integer> xposForLevel = new HashMap<>();
    Map<TreeNode<T>, LayoutData> layoutMap = new HashMap<>();

    float xTopAdjustment;
    float yTopAdjustment;

    // maintain a list of neighboring nodes on each level
    // this seems to maintain the left neighbors of the current sub tree only ...

    private Map<Integer, TreeNode<T>> leftNeighbor = new HashMap<>();

    /**
     * @param nLevelNbr
     *
     * @return The left neighbor at the given tree level.
     */
    private TreeNode<T> GetPrevNodeAtLevel (int nLevelNbr) {
        return leftNeighbor.get(nLevelNbr);
    }

    
    /**
     * Stores the given node as the left neighbor at the current level.
     * 
     * @param nLevelNbr
     * @param pThisNode
     */
    void SetPrevNodeAtLevel (int nLevelNbr, TreeNode<T> pThisNode) {
        leftNeighbor.put(nLevelNbr, pThisNode);
    }
    
    
    private void calculateFinalX(TreeNode<T> node, float modSum) {
        LayoutData ldChild = layoutMap.get(node);
        if (ldChild == null) {
            ldChild = new LayoutData();
            layoutMap.put(node, ldChild);
        }

        ldChild.xCoordinate += modSum;
        modSum += ldChild.flModifier;

        for (TreeNode<T> child : node.getChildren()) {
            calculateFinalX(child, modSum);
        }
    }
    
    float shiftAmount = 0;
    private void CheckAllChildrenOnScreen(TreeNode<T> rootNode) {
        LayoutData ld = layoutMap.get(rootNode);
        if (ld == null) {
            ld = new LayoutData();
            layoutMap.put(rootNode, ld);
        }

        Map<Integer, Float> nodeContour = new HashMap<>();
        GetLeftContour(rootNode, 0, nodeContour);

        // float 
        shiftAmount = 0;
        nodeContour.keySet().forEach( y -> {
            if (nodeContour.get(y) + shiftAmount < 0) {
                shiftAmount = (nodeContour.get(y) * -1);
            }
        });
     
        if (shiftAmount > 0) {
            ld.xCoordinate += shiftAmount;
            ld.flModifier += shiftAmount;
        }
    }


    /**
     * 
     * @param node
     * @param modSum
     * @param values
     */
    private void GetLeftContour(TreeNode<T> node, float modSum, Map<Integer, Float> values) {
        LayoutData ld = getLayout(node);

        if (!values.containsKey(ld.yCoordinate)) {
            values.put(ld.yCoordinate, ld.xCoordinate + modSum);
        }
        else {
            values.put(ld.yCoordinate, Math.min(values.get(ld.yCoordinate), ld.xCoordinate + modSum));
        }

        modSum += ld.flModifier;
        for (TreeNode<T> child : node.getChildren()) {
            GetLeftContour(child, modSum, values);
        }
    }
    
    

    private void GetRightContour(TreeNode<T> node, float modSum, Map<Integer, Float> values) {
        LayoutData ld = getLayout(node);

        if (!values.containsKey(ld.yCoordinate)) {
            values.put(ld.yCoordinate, ld.xCoordinate + modSum);
        }
        else {
            values.put(ld.yCoordinate, Math.max(values.get(ld.yCoordinate), ld.xCoordinate + modSum));
        }

        modSum += ld.flModifier;
        for (TreeNode<T> child : node.getChildren()) {
            GetLeftContour(child, modSum, values);
        }
    }


    private LayoutData getLayout(TreeNode<T> node) {
        LayoutData layoutData = null;
        if (node != null) { 
            layoutData = layoutMap.get(node);
            if (layoutData == null) {
                layoutData = new LayoutData();
                layoutMap.put(node, layoutData);
            }
        }

        return layoutData;
    }

    
    private float TreeMeanNodeSize (LayoutData pLeftNode,
                                    LayoutData pRightNode) {
        return 80;
    }


    private void TreeFirstWalk(TreeNode<T> pThisNode) {
        LayoutData pThisNodeLayout = getLayout(pThisNode);
        

        pThisNodeLayout.prev = GetPrevNodeAtLevel(pThisNode.getLevel());
        SetPrevNodeAtLevel(pThisNode.getLevel(), pThisNode);

//      pThisNode->prev = GetPrevNodeAtLevel(pThisNode.getLevel());

        
        
//      System.err.println(" => " + pThisNode);

      /* Clean up old values in a node's flModifier         */
      pThisNodeLayout.flModifier = (float)0.0;

      if (pThisNode.isLeaf()) { // || (nCurrentLevel == MAXIMUM_DEPTH)

          TreeNode<T> leftSibling = pThisNode.getLeftSibling();
          if (leftSibling != null) {
              LayoutData leftSiblingLayout = getLayout(leftSibling);

              float flMeanWidth = TreeMeanNodeSize(leftSiblingLayout, pThisNodeLayout);
              pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + 5 + flMeanWidth;
          } else {
              pThisNodeLayout.flPrelim = 0;     /* no sibling on the left to worry about */
          }

      } else {
          TreeNode<T> pLeftmost = pThisNode.getFirstChild();
          TreeNode<T> pRightmost = pLeftmost; // pThisNode.getLastChild();
          
          TreeFirstWalk(pLeftmost);
          
          while(pRightmost.getRightSibling() != null) {
              pRightmost = pRightmost.getRightSibling();
              TreeFirstWalk(pRightmost);
          }

          LayoutData pLeftmostLayout = getLayout(pLeftmost);
          LayoutData pRightmostLayout = getLayout(pRightmost);

          float flMidpoint = (pLeftmostLayout.flPrelim + pRightmostLayout.flPrelim) / 2;
          float flMeanWidth = TreeMeanNodeSize(pLeftmostLayout, pRightmostLayout);
          
          TreeNode<T> leftSibling = pThisNode.getLeftSibling();
          if (leftSibling != null) {
              LayoutData leftSiblingLayout = getLayout(leftSibling);

              pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + 5 + flMeanWidth;
              pThisNodeLayout.flModifier = pThisNodeLayout.flPrelim - flMidpoint;

              TreeApportion(pThisNode); // , nCurrentLevel);
          } else {
              pThisNodeLayout.flPrelim = flMidpoint;
          }
          
      }
        
    }

    
    private TreeNode<T> TreeGetLeftmost(TreeNode<T> pThisNode,
            int nCurrentLevel,
            int nSearchDepth)  {
        /*------------------------------------------------------
        * Determine the leftmost descendant of a node at a
        * given depth. This is implemented using a post-order
        * walk of the subtree under pThisNode, down to the
        * level of nSearchDepth. If we've searched to the
        * proper distance, return the currently leftmost node.
        * Otherwise, recursively look at the progressively
        * lower levels.
        *----------------------------------------------------*/
        
        TreeNode<T> pLeftmost;    /* leftmost descendant at level   */
        TreeNode<T> pRightmost;   /* rightmost offspring in search  */
        
        if (nCurrentLevel == nSearchDepth) {
            pLeftmost = pThisNode; /*  searched far enough.    */
        } else if (pThisNode.isLeaf()) {
            pLeftmost = null;  /* This node has no descendants    */
        } else {  /* Do a post-order walk of the subtree.        */
                for (pLeftmost = TreeGetLeftmost(pRightmost =
                      pThisNode.getFirstChild(),
                      nCurrentLevel + 1,
                      nSearchDepth);
                     (pLeftmost == null) && (pRightmost.getRightSibling() != null);
                        pLeftmost = TreeGetLeftmost(pRightmost =pRightmost.getRightSibling(), nCurrentLevel + 1, nSearchDepth)
        ) { /* Nothing inside this for-loop. */ }
        }

        return (pLeftmost);
    }


    private void TreeApportion(TreeNode<T> pThisNode) {
//        TreeNode<T> pLeftmost;            /* leftmost at given level*/
//        PNODE pNeighbor;            /* node left of pLeftmost */
        TreeNode<T> pAncestorLeftmost;    /* ancestor of pLeftmost  */
        TreeNode<T> pAncestorNeighbor;    /* ancestor of pNeighbor  */
        TreeNode<T> pTempPtr;             /* loop control pointer   */
//        unsigned i;                 /* loop control           */
//        unsigned nCompareDepth;     /* depth of comparison    */
//                               /* within this proc       */
//        unsigned nDepthToStop;      /* depth to halt          */
       int nLeftSiblings;     /* nbr of siblings to the */
//                /* left of pThisNode, including pThisNode,  */
//                /* til the ancestor of pNeighbor            */
        float flLeftModsum;         /* sum of ancestral mods  */
        float flRightModsum;        /* sum of ancestral mods  */
        float flDistance;           /* difference between     */
//             /* where pNeighbor thinks pLeftmost should be   */
//             /* and where pLeftmost actually is              */
        float flPortion;            /* proportion of          */
//             /* flDistance to be added to each sibling       */

        TreeNode<T> pLeftmost = pThisNode.getFirstChild();
        TreeNode<T> pNeighbor = getLayout(pLeftmost).prev; //  .getLeftSibling(); // !!!!!!!!!!! LeftNeighbor(pLeftmost);

        int nCompareDepth = 1;
        int nDepthToStop = /* MAXIMUM_DEPTH */ 20 - pThisNode.getLevel(); // nCurrentLevel;

        while ((pLeftmost != null) && (pNeighbor != null) &&
              (nCompareDepth <= nDepthToStop)) {

           /* Compute the location of pLeftmost and where it */
           /* should be with respect to pNeighbor.           */
           flRightModsum = flLeftModsum = (float)0.0;
           pAncestorLeftmost = pLeftmost;
           pAncestorNeighbor = pNeighbor;
           for (int i = 0; (i < nCompareDepth); i++) {
              pAncestorLeftmost = pAncestorLeftmost.getParent();
              pAncestorNeighbor = pAncestorNeighbor.getParent();
              flRightModsum = flRightModsum + getLayout(pAncestorLeftmost).flModifier;
              flLeftModsum = flLeftModsum + getLayout(pAncestorNeighbor).flModifier;
           }

           /* Determine the flDistance to be moved, and apply*/
           /* it to "pThisNode's" subtree.  Apply appropriate*/
           /* portions to smaller interior subtrees          */

           /* Set the global mean width of these two nodes   */
           float flMeanWidth = TreeMeanNodeSize(getLayout(pLeftmost), getLayout(pNeighbor));

           flDistance = (getLayout(pNeighbor).flPrelim +
                       flLeftModsum +
                       (float)80 + // SUBTREE_SEPARATION +
                       (float)flMeanWidth) -
                      (getLayout(pLeftmost).flPrelim + flRightModsum);

           if (flDistance > (float)0.0) {
              /* Count the interior sibling subtrees        */
              nLeftSiblings = 0;
              for (pTempPtr = pThisNode;
                   (pTempPtr != null) &&
                   (pTempPtr != pAncestorNeighbor);
                  pTempPtr = pTempPtr.getLeftSibling()) { // Leftsibling(pTempPtr)) {
                 nLeftSiblings++;
              }

              if (pTempPtr != null) {
                 /* Apply portions to appropriate          */
                 /* leftsibling subtrees.                  */
                 flPortion = flDistance/(float)nLeftSiblings;
                 for (pTempPtr = pThisNode;
                     (pTempPtr != pAncestorNeighbor);
                     pTempPtr = pTempPtr.getLeftSibling()) {
                     getLayout(pTempPtr).flPrelim = getLayout(pTempPtr).flPrelim + flDistance;
                    getLayout(pTempPtr).flModifier = getLayout(pTempPtr).flModifier + flDistance;
                    flDistance = flDistance - flPortion;
                  }
              }
              else {
                 /* Don't need to move anything--it needs  */
                 /* to be done by an ancestor because      */
                 /* pAncestorNeighbor and                  */
                 /* pAncestorLeftmost are not siblings of  */
                 /* each other.                            */
                 return;
              }
           }  /* end of the while                           */

           /* Determine the leftmost descendant of pThisNode */
           /* at the next lower level to compare its         */
           /* positioning against that of its pNeighbor.     */

           nCompareDepth++;
           if (pLeftmost.isLeaf()) {
              pLeftmost = TreeGetLeftmost(pThisNode, 0,
                                          nCompareDepth);
           } else {
              pLeftmost = pLeftmost.getFirstChild();
           }
           
           if (pLeftmost != null) 
           pNeighbor = getLayout(pLeftmost).prev; // .getLeftSibling();  // !!!!!!!!!!!1 LeftNeighbor(pLeftmost);
        }
    }

    private float flModsum = (float)0.0;

    private void TreeSecondWalk(TreeNode<T> pThisNode2) {
     LayoutData pThisNode2Layout = getLayout(pThisNode2);
     int nCurrentLevel = pThisNode2.getLevel();

     float lxTemp, lyTemp;        /* hold calculations here */
     float flNewModsum;          /* local modifier value   */
     
//     // if (nCurrentLevel <= MAXIMUM_DEPTH) {
     flNewModsum = flModsum;
     
     lxTemp = (long)xTopAdjustment + (long)(pThisNode2Layout.flPrelim + flModsum);
     lyTemp = (long)yTopAdjustment + (long)(nCurrentLevel * 80); // LEVEL_SEPARATION);

//
     // if (CheckExtentsRange(lxTemp, lyTemp)) {
         /* The values are within the allowable range */
     pThisNode2Layout.xCoordinate = lxTemp;
     pThisNode2Layout.yCoordinate = (int) lyTemp;

     if (!pThisNode2.isLeaf()) {
         /* Apply the flModifier value for this    */
         /* node to all its offspring.             */
         flModsum = flNewModsum = flNewModsum + pThisNode2Layout.flModifier;
         /*bResult = */ TreeSecondWalk(pThisNode2.getFirstChild()); // FirstChild(pThisNode), nCurrentLevel + 1);

         flNewModsum = flNewModsum - pThisNode2Layout.flModifier;
      }

      TreeNode<T> rightSibling = pThisNode2.getRightSibling(); 
      if (rightSibling != null) { // && (bResult)) {
         flModsum = flNewModsum;
         /*bResult = */ TreeSecondWalk(rightSibling); // , nCurrentLevel);
      }
    }
    
    
    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("JavaFX Tree viewer example");
       

        Pane mainLayout = new Pane();

        // http://www.drdobbs.com/positioning-nodes-for-general-trees/184402320?pgno=4
        // https://rachel53461.wordpress.com/2014/04/20/algorithm-for-drawing-trees/

        // First walk
//      TreeTraversal<T> traversal1 = new DepthFirstTraversal<>();
//      traversal1.traversePostOrder(rootNode, (pThisNode) -> {
//          System.err.println(" => " + pThisNode);
//          LayoutData pThisNodeLayout = getLayout(pThisNode);
//
//          /* Clean up old values in a node's flModifier         */
//          pThisNodeLayout.flModifier = (float)0.0;
//
//          if (pThisNode.isLeaf()) { // || (nCurrentLevel == MAXIMUM_DEPTH)
//
//              TreeNode<T> leftSibling = pThisNode.getLeftSibling();
//              if (leftSibling != null) {
//                  LayoutData leftSiblingLayout = getLayout(leftSibling);
//
//                  float flMeanWidth = TreeMeanNodeSize(leftSiblingLayout, pThisNodeLayout);
//                  pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + 5 + flMeanWidth;
//              } else {
//                  pThisNodeLayout.flPrelim = 0;     /* no sibling on the left to worry about */
//              }
//
//          } else {
//
//              TreeNode<T> pLeftmost = pThisNode.getFirstChild();
//              TreeNode<T> pRightmost = pThisNode.getLastChild();
//              LayoutData pLeftmostLayout = getLayout(pLeftmost);
//              LayoutData pRightmostLayout = getLayout(pRightmost);
//
//              float flMidpoint = (pLeftmostLayout.flPrelim + pRightmostLayout.flPrelim) / 2;
//              float flMeanWidth = TreeMeanNodeSize(pLeftmostLayout, pRightmostLayout);
//              
//              TreeNode<T> leftSibling = pThisNode.getLeftSibling();
//              if (leftSibling != null) {
//                  LayoutData leftSiblingLayout = getLayout(leftSibling);
//
//                  pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + 5 + flMeanWidth;
//                  pThisNodeLayout.flModifier = pThisNodeLayout.flPrelim - flMidpoint;
//                  // TreeApportion(pThisNode, nCurrentLevel);
//              } else {
//                  pThisNodeLayout.flPrelim = flMidpoint;
//              }
//              
//          }
//      });
//
//          
     TreeFirstWalk(rootNode);

     LayoutData ldRoot = getLayout(rootNode);
     xTopAdjustment = ldRoot.xCoordinate;
     yTopAdjustment = ldRoot.yCoordinate;

     TreeSecondWalk(rootNode);

//          // Second walk
//     traversal1.traversePreOrder(rootNode, (pThisNode2) -> {
//
//         LayoutData pThisNode2Layout = getLayout(pThisNode2);
//         int nCurrentLevel = pThisNode2.getLevel();
//
//         float lxTemp, lyTemp;        /* hold calculations here */
//         float flNewModsum;          /* local modifier value   */
//         
////         // if (nCurrentLevel <= MAXIMUM_DEPTH) {
//         flNewModsum = flModsum;
//         
//         lxTemp = (long)xTopAdjustment + (long)(pThisNode2Layout.flPrelim + flModsum);
//         lyTemp = (long)yTopAdjustment + (long)(nCurrentLevel * 80); // LEVEL_SEPARATION);
//
////
//         // if (CheckExtentsRange(lxTemp, lyTemp)) {
//             /* The values are within the allowable range */
//         pThisNode2Layout.xCoordinate = lxTemp;
//         pThisNode2Layout.yCoordinate = (int) lyTemp;
//
//         if (!pThisNode2.isLeaf()) {
//             /* Apply the flModifier value for this    */
//             /* node to all its offspring.             */
//             flModsum = flNewModsum = flNewModsum + pThisNode2Layout.flModifier;
//             /*bResult = TreeSecondWalk(
//                FirstChild(pThisNode),nCurrentLevel + 1);*/
//
////             flNewModsum = flNewModsum -
////                         pThisNode->flModifier;
//          }
//
//          TreeNode<T> rightSibling = pThisNode2.getRightSibling(); 
//          if (rightSibling != null) { // && (bResult)) {
//             flModsum = flNewModsum;
//             //bResult = TreeSecondWalk(
//             //    RightSibling(pThisNode), nCurrentLevel);
//          }
//
////         flModsum = flNewModsum = flNewModsum + pThisNode->flModifier;
//     });

//
//          // child nodes have already been processed!
//          
//
//          // iterate all child nodes and assign initial values
//         int x = 0;
//         for (TreeNode<T> tn : pThisNode.getChildren()) {
//             LayoutData ldChild = layoutMap.get(tn);
//             if (ldChild == null) {
//                 ldChild = new LayoutData();
//                 layoutMap.put(tn, ldChild);
//             }
//
//             ldChild.Y = tn.getLevel();
//             ldChild.X = ldChild.X + 1.0F;
//         }
//
//         
//         // Positioning Child Nodes under Parents
//
//         // calculate midpoint
//         TreeNode<T> first = pThisNode.getFirstChild();
//         if (first != null) {
//             LayoutData ldFirst = layoutMap.get(first);
//
//             float midpoint = ldFirst.X;
//
//             TreeNode<T> last = pThisNode.getLastChild();
//             if (last != null) {
//                 LayoutData ldLast = layoutMap.get(last);
//                 midpoint = (ldFirst.X + ldLast.X) / 2;
//             }
//
//             LayoutData ldCurrent = layoutMap.get(pThisNode);
//             if (ldCurrent == null) {
//                 ldCurrent = new LayoutData();
//                 layoutMap.put(pThisNode, ldCurrent );
//             }
//             
//             if (pThisNode.getLeftSibling() == null) {
//                 ldCurrent.X = midpoint;    
//             } else {
//                 ldCurrent.flModifier = ldCurrent.X - midpoint;
//             }
//             
//         }
//      });
//
//      
//      
//      
//      
//      // Do a second walk through the tree to determine that no children will be drawn off-screen, and adjust the Mod property if needed. This can happen when if the Mod property is negative.
//     CheckAllChildrenOnScreen(rootNode);
//
//
//      // Step 2: ...
//      //TreeTraversal<T> traversal1 = new DepthFirstTraversal<>();
////      traversal1.traversePreOrder(rootNode, (node) -> {
////          LayoutData ldChild = layoutMap.get(node);
////          if (ldChild == null) {
////              ldChild = new LayoutData();
////              layoutMap.put(node, ldChild);
////          }
////
////          ldChild.mod = 3;
////      });
//
//      // Step 3: Calculate final positions
//      calculateFinalX(rootNode, 0);

      // TreeTraversal<T> traversal3 = new DepthFirstTraversal<>();
//      
//      traversal1.traversePreOrder(rootNode, (node) -> {
//
//          LayoutData ldChild = layoutMap.get(node);
//          if (ldChild == null) {
//              ldChild = new LayoutData();
//              layoutMap.put(node, ldChild);
//          }
//          
//          
//      });
//
        
        // alle knoten eines Levels sind in einer Liste gespeichert
        // Jeden Knoten nehmen (unterster Level zuerst)
        // - Zentriere Knoten und seine Siblings unter dem Parent-Knoten
        
        // Jeder Knoten braucht so viel Platz dass seine Kind-Knoten Platz haben - rekursiv!
        // => gesamt-Breite jedes Knotens berechnen
        // => Wie??

//        // Bottom-up-traversal to calculate the final width of each sub tree 
//        // (maximum number of siblings on one level)
//        TreeTraversal<T> tt3 = new DepthFirstTraversal<>();
//        tt3.traversePostOrder(rootNode, (node) -> {
//           LayoutData ld = new LayoutData();
//           layoutMap.put(node, ld);
//
//           for (TreeNode<T> tn : node.getChildren()) {
//               LayoutData ldChild = layoutMap.get(tn);
//               ld.width += ldChild.width;
//           }
//
//           if (ld.width > 1) {
//               ld.width--;
//           }
//        });
//
//
//        TreeTraversal<T> tt4 = new BreadthFirstTraversal<>();
//        tt4.traversePreOrder(rootNode, (node) -> {
//            LayoutData ld = layoutMap.get(node);
//
//            int width = ld.width;
//
//            int level = node.getLevel();
//            Integer xpos = xposForLevel.get(level);
//            if (xpos == null) {
//                xpos = (width-1) / 2;
//            }
//
//            ld.column = xpos; //* 80;
//            ld.row = level; //  * 80;
//
//            xpos += width; //  * 80;
//            xposForLevel.put(level, xpos);
//        });


        // Render the nodes and edges
        TreeTraversal<T> tt5 = new DepthFirstTraversal<>();
        tt5.traversePreOrder(rootNode, (node) -> {
            LayoutData layoutData = getLayout(node);
            if (layoutData == null) return;

            Label label = new Label(node.toString());
            label.setLayoutX(layoutData.xCoordinate);
            label.setLayoutY(layoutData.yCoordinate);

            label.setStyle("-fx-padding: 5px; -fx-background-insets: 5px; -fx-background-color: yellow; -fx-border-style: solid;");
            mainLayout.getChildren().add(label);

/*
            Circle center = new Circle();
            center.setLayoutX(layoutData.xCoordinate);
            center.setLayoutY(layoutData.yCoordinate);
            center.setStroke(Color.RED);
            center.setRadius(3.0);
            mainLayout.getChildren().add(center);
*/
            Text t = new Text(String.format("%s/%s - %s",  layoutData.xCoordinate, layoutData.yCoordinate, layoutData.flModifier));
            t.setLayoutX(layoutData.xCoordinate + 10);
            t.setLayoutY(layoutData.yCoordinate);
            mainLayout.getChildren().add(t);

            TreeNode<T> parent = node.getParent();
            if (parent != null) { 
                LayoutData layoutDataP = getLayout(parent);
                if (layoutDataP == null) return;

                Line edge = new Line();
                edge.setStartX(layoutDataP.xCoordinate + 30);
                edge.setStartY(layoutDataP.yCoordinate + 29);

                edge.setEndX(layoutData.xCoordinate + 30);
                edge.setEndY(layoutData.yCoordinate);
                mainLayout.getChildren().add(edge);
            }
        });

        // show the generated scene graph
        Scene scene = new Scene(mainLayout);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    public static void main(TreeNode<String> tree) {
        staticNode = tree;    // HACK!
        launch(new String[] {});
    }
}
