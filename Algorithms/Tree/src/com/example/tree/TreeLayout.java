package com.example.tree;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Function;

import javafx.scene.Node;
import javafx.scene.layout.Region;
import javafx.scene.layout.Pane;
import javafx.scene.shape.Line;


/**
 * Implements a layout algorithm to layout an n-ary tree.
 *
 *   https://blogs.oracle.com/jfxprg/entry/dynamical_layouts_in_fx
 *   http://www.drdobbs.com/positioning-nodes-for-general-trees/184402320?pgno=2
 *   https://rachel53461.wordpress.com/2014/04/20/algorithm-for-drawing-trees/
 *
 * @param <T>
 */
public class TreeLayout<T> extends Region {

    private final TreeNode<T> rootNode;
    private final Function<TreeNode<T>, Node> createNode;   // factory method to create a Node representation
    private final Function<TreeNode<T>, Node> createEdge;   // factory method to create an Edge representation

    private static final float HORIZONTAL_SPACING = 5.0F;   // the spacing between adjacent siblings
    private static final float SUBTREE_SEPARATION = 20.0F;  // The miminal distance between sub trees
    private static final int LEVEL_SEPARATION = 120;        // the height of one level

    private boolean needsLayout = true;
    private double maxWidth = 0;
    private double maxHeight = 0;

    private float xTopAdjustment;
    private float yTopAdjustment;

    private boolean flag = true;

    // maintain a list of neighboring nodes on each level
    // this map maintains the left neighbors of the current sub tree
    // so that in a later step the proper node can be assigned as "left neighbor" 
    private Map<Integer, TreeNode<T>> leftNeighbor = new HashMap<>();

    private float flModsum = 0.0F;


    /**
     * 
     * @param rootNode
     */
    public TreeLayout(TreeNode<T> rootNode, 
                      Function<TreeNode<T>, Node> createNode,
                      Function<TreeNode<T>, Node> createLine) {
        this.rootNode = rootNode;
        this.createNode = createNode;
        this.createEdge = createLine;
        
        getStyleClass().add("TreeLayout");
    }


    private void doCreateLayout() {
        // initialize the graphical artifacts.
        // after this traversal, each node has a LayoutData assigned which contains 
        // the node and the parent path.
        
        List<Node> nodes = new ArrayList<>();
        List<Node> edges = new ArrayList<>();

        TreeTraversal<T> tt5 = new DepthFirstTraversal<>();
        tt5.traversePreOrder(rootNode, (node) -> {
            // initialize layout data for this node.
            LayoutDataImpl<T> layoutData = new LayoutDataImpl<>();
            node.setLayoutData(layoutData);

            // create the node
            Node nodeView = createNode.apply(node);
            nodeView.setOnMouseClicked(e -> {System.err.println(node); });
            layoutData.nodePanel = (Pane) nodeView;
            nodes.add(nodeView);

            // create the edge to the parent
            TreeNode<T> parent = node.getParent();
            if (parent != null) {
                Node edgeView = createEdge.apply(node);
                layoutData.edgeLine = (Line) edgeView;
                edges.add(edgeView);
            }
        });


        // first, add all edges so that they are behind the nodes
        for (Node edge : edges) {
            getChildren().add(edge);
            
        }
        for (Node node1 : nodes) {
            getChildren().add(node1);
        }

        this.applyCss();        // !!!!! Required in order to consider CSS layout properties!!!!
        calculateLayout();
        
//        TreeTraversal<T> dump = new DepthFirstTraversal<>();
//        dump.traversePreOrder(rootNode, (node) -> {
//            System.err.println(node + " => " + node.getLayoutData());
//        });
    }


    /**
     * @param nLevelNbr
     *
     * @return The left neighbor at the given tree level.
     */
    private TreeNode<T> GetPrevNodeAtLevel(int nLevelNbr) {
        return leftNeighbor.get(nLevelNbr);
    }

    /**
     * Stores the given node as the left neighbor at the current level.
     * 
     * @param nLevelNbr
     * @param pThisNode
     */
    private void SetPrevNodeAtLevel(int nLevelNbr, TreeNode<T> pThisNode) {
        leftNeighbor.put(nLevelNbr, pThisNode);
    }


    /**
     * 
     * @param pLeftNode
     * @param pRightNode
     *
     * @return
     */
    private float treeMeanNodeSize(TreeNode<T> pLeftNode, TreeNode<T> pRightNode) {
        float result = 0;

        if (pLeftNode != null) {
            LayoutDataImpl<T> pLeftNodeLayout = (LayoutDataImpl<T>) pLeftNode.getLayoutData();
            result = (float) pLeftNodeLayout.getWidth() / 2;       // !!!!!!!!! seems ok
        }

        if (pRightNode != null) {
            LayoutDataImpl<T> pRightNodeLayout = (LayoutDataImpl<T>) pRightNode.getLayoutData();
            result = result + (float) pRightNodeLayout.getWidth() / 2;       // !!!!!!!!! seems ok
        }

        // System.err.println("TreeMeanNodeSize() = " + result);
        return result;      // need to consider spacing also!!
    }

    /**
     * Post-order traversal of the tree.
     * Calculates the preliminary X position of each node
     *
     * @param pThisNode
     */
    private void treeFirstWalk(TreeNode<T> pThisNode) {
        LayoutDataImpl<T> pThisNodeLayout = (LayoutDataImpl<T>) pThisNode.getLayoutData();

        // calculate the "left neighbor" reference.
        pThisNodeLayout.setLeftNeighbor(GetPrevNodeAtLevel(pThisNode.getLevel()));
        SetPrevNodeAtLevel(pThisNode.getLevel(), pThisNode);

        // Clean up old values in a node's flModifier
        pThisNodeLayout.flModifier = (float) 0.0;

        if (pThisNode.isLeaf()) { // || (nCurrentLevel == MAXIMUM_DEPTH)

            // calculate the preliminary (relative) X coordinate for a leaf node.
            // * The x position is 0 if the node has no left sibling
            // * Otherwise, the x position is the x coordinate of the left sibling
            //   plus the width of the left sibling plus the horizontal spacing
            TreeNode<T> leftSibling = pThisNode.getLeftSibling();
            if (leftSibling != null) {
                LayoutDataImpl<T> leftSiblingLayout = (LayoutDataImpl<T>) leftSibling.getLayoutData();

                float flMeanWidth = (float) leftSiblingLayout.getWidth(); // treeMeanNodeSize(leftSibling, pThisNode);
                pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + HORIZONTAL_SPACING + flMeanWidth;
            } else {
                // no sibling on the left, preliminary x position is 0
                pThisNodeLayout.flPrelim = 0;
            }

        } else {    // Not a leaf node. Calculate midpoint between its children.

            // walk through all child nodes
            for (TreeNode<T> child : pThisNode.getChildren()) {
                treeFirstWalk(child);
            }

            TreeNode<T> pLeftmost = pThisNode.getFirstChild();
            TreeNode<T> pRightmost = pThisNode.getLastChild();

            LayoutDataImpl<T> pLeftmostLayout = (LayoutDataImpl<T>) pLeftmost.getLayoutData();
            LayoutDataImpl<T> pRightmostLayout = (LayoutDataImpl<T>) pRightmost.getLayoutData();
            
            // calculate the X position of the parent node
            double leftX = pLeftmostLayout.flPrelim;
            double rightX = pRightmostLayout.flPrelim + pRightmostLayout.getWidth();
            double midPoint = (leftX + rightX) / 2;
            float flMidpoint = (float) (midPoint - pThisNodeLayout.getWidth() / 2);

            // If the current node has a left sibling, then calculate its preliminary position
            // based on the left sibling.
            TreeNode<T> leftSibling = pThisNode.getLeftSibling();
            if (leftSibling != null) {
                LayoutDataImpl<T> leftSiblingLayout = (LayoutDataImpl<T>) leftSibling.getLayoutData();

                float flMeanWidth = treeMeanNodeSize(pLeftmost, pThisNode);
                pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + HORIZONTAL_SPACING + flMeanWidth;
                pThisNodeLayout.flModifier = pThisNodeLayout.flPrelim - flMidpoint;

                // move the sub trees of this node to the proper location
                treeApportion(pThisNode);
            } else {
                // no sibling on the left, preliminary x position is the midpoint between 
                // its leftmost and rightmost child
                pThisNodeLayout.flPrelim = flMidpoint;
            }

        }
    }


    private TreeNode<T> TreeGetLeftmost(TreeNode<T> pThisNode, int nCurrentLevel, int nSearchDepth) {
        /*------------------------------------------------------
        * Determine the leftmost descendant of a node at a
        * given depth. This is implemented using a post-order
        * walk of the subtree under pThisNode, down to the
        * level of nSearchDepth. If we've searched to the
        * proper distance, return the currently leftmost node.
        * Otherwise, recursively look at the progressively
        * lower levels.
        *----------------------------------------------------*/

        TreeNode<T> pLeftmost; /* leftmost descendant at level */
        TreeNode<T> pRightmost; /* rightmost offspring in search */

        if (nCurrentLevel == nSearchDepth) {
            pLeftmost = pThisNode; /* searched far enough. */
        } else if (pThisNode.isLeaf()) {
            pLeftmost = null; /* This node has no descendants */
        } else { /* Do a post-order walk of the subtree. */
            for (pLeftmost = TreeGetLeftmost(pRightmost = pThisNode.getFirstChild(), nCurrentLevel + 1,
                    nSearchDepth); (pLeftmost == null)
                            && (pRightmost.getRightSibling() != null); pLeftmost = TreeGetLeftmost(
                                    pRightmost = pRightmost.getRightSibling(), nCurrentLevel + 1, nSearchDepth)) {
                /* Nothing inside this for-loop. */ }
        }

        return (pLeftmost);
    }


    /**
     * Moves the sub trees to the proper locations to the right
     *
     * @param pThisNode
     */
    private void treeApportion(TreeNode<T> pThisNode) {
        int nCompareDepth = 1;
        int nDepthToStop = /* MAXIMUM_DEPTH */ 20 - pThisNode.getLevel();

        TreeNode<T> pLeftmost = pThisNode.getFirstChild();
        TreeNode<T> pNeighbor = ((LayoutDataImpl<T>) pLeftmost.getLayoutData()).getLeftNeighbor();
        while ((pLeftmost != null) && (pNeighbor != null) && (nCompareDepth <= nDepthToStop)) {

            // Compute the location of pLeftmost and where it
            // should be with respect to pNeighbor.
            float flRightModsum = 0.0F;         // sum of ancestral mods
            float flLeftModsum = 0.0F;          // sum of ancestral mods
            TreeNode<T> pAncestorLeftmost = pLeftmost;  // ancestor of pLeftmost
            TreeNode<T> pAncestorNeighbor = pNeighbor;  // ancestor of pNeighbor
            for (int i = 0; (i < nCompareDepth); i++) {
                pAncestorLeftmost = pAncestorLeftmost.getParent();
                pAncestorNeighbor = pAncestorNeighbor.getParent();
                flRightModsum = flRightModsum + ((LayoutDataImpl<T>) pAncestorLeftmost.getLayoutData()).flModifier;
                flLeftModsum = flLeftModsum + ((LayoutDataImpl<T>) pAncestorNeighbor.getLayoutData()).flModifier;
            }

            /* Determine the flDistance to be moved, and apply */
            /* it to "pThisNode's" subtree. Apply appropriate */
            /* portions to smaller interior subtrees */

            /* Set the global mean width of these two nodes */
            float flMeanWidth = treeMeanNodeSize(pLeftmost, pNeighbor);

            float flDistance =  // difference between where pNeighbor thinks pLeftmost should be
                                // and where pLeftmost actually is
                    ( ((LayoutDataImpl<T>) pNeighbor.getLayoutData()).flPrelim + flLeftModsum +  
                      SUBTREE_SEPARATION +
                      (float) flMeanWidth  ) - 
                    ( ((LayoutDataImpl<T>)pLeftmost.getLayoutData()).flPrelim + flRightModsum );

            if (flDistance > 0.0F) {
                int nLeftSiblings = 0;  // nbr of siblings to the
                                        // left of pThisNode, including pThisNode,
                                        // til the ancestor of pNeighbor

                // Count the interior sibling subtrees
                TreeNode<T> pTempPtr; /* loop control pointer */
                for (pTempPtr = pThisNode; 
                     (pTempPtr != null) && (pTempPtr != pAncestorNeighbor); 
                     pTempPtr = pTempPtr.getLeftSibling()) {
                    nLeftSiblings++;
                }

                if (pTempPtr != null) {
                    /* Apply portions to appropriate */
                    /* leftsibling subtrees. */

                    // proportion of flDistance to be added to each sibling
                    final float flPortion =  flDistance / (float) nLeftSiblings;

                    for (pTempPtr = pThisNode; (pTempPtr != pAncestorNeighbor); pTempPtr = pTempPtr.getLeftSibling()) {
                        ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flPrelim = ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flPrelim + flDistance;
                        ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flModifier = ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flModifier + flDistance;
                        flDistance = flDistance - flPortion;
                    }
                } else {
                    // Don't need to move anything--it needs
                    // to be done by an ancestor because pAncestorNeighbor and
                    // pAncestorLeftmost are not siblings of each other.
                    return;
                }
            }

            // Determine the leftmost descendant of pThisNode
            // at the next lower level to compare its
            // positioning against that of its pNeighbor.

            nCompareDepth++;
            if (pLeftmost.isLeaf()) {
                pLeftmost = TreeGetLeftmost(pThisNode, 0, nCompareDepth);
            } else {
                pLeftmost = pLeftmost.getFirstChild();
            }

            if (pLeftmost != null) {
                pNeighbor = ((LayoutDataImpl<T>) pLeftmost.getLayoutData()).getLeftNeighbor();
            }
        }
    }


    /**
     * Calculate final X/Y coordinates for each tree node.
     *
     * @param pThisNode
     */
    private void treeSecondWalk(TreeNode<T> pThisNode) {

        // finally set the node and edge positions
        LayoutDataImpl<T> pThisNodeLayout = (LayoutDataImpl<T>) pThisNode.getLayoutData();
        int nCurrentLevel = pThisNode.getLevel();

        // // if (nCurrentLevel <= MAXIMUM_DEPTH) {
        float flNewModsum = flModsum;

        final float lxTemp = (long) xTopAdjustment + (long) (pThisNodeLayout.flPrelim + flModsum);
        final float lyTemp = (long) yTopAdjustment + (long) (nCurrentLevel * LEVEL_SEPARATION);

        //
        // if (CheckExtentsRange(lxTemp, lyTemp)) {
        /* The values are within the allowable range */

        pThisNodeLayout.xCoordinate = lxTemp;
        pThisNodeLayout.yCoordinate = lyTemp;

        maxWidth = Math.max(maxWidth, lxTemp + pThisNodeLayout.getWidth());
        maxHeight = Math.max(maxHeight, lyTemp +pThisNodeLayout.getHeight());

        if (!pThisNode.isLeaf()) {
            // Apply the flModifier value for this node to all its offspring.
            flModsum = flNewModsum = flNewModsum + pThisNodeLayout.flModifier;
            treeSecondWalk(pThisNode.getFirstChild());
            flNewModsum = flNewModsum - pThisNodeLayout.flModifier;
        }

        TreeNode<T> rightSibling = pThisNode.getRightSibling();
        if (rightSibling != null) {
            flModsum = flNewModsum;
            treeSecondWalk(rightSibling);
        }
    }


    private void calculateLayout() {
        treeFirstWalk(rootNode);

        LayoutDataImpl<T> ldRoot = (LayoutDataImpl<T>) rootNode.getLayoutData();
        xTopAdjustment = ldRoot.xCoordinate;
        yTopAdjustment = ldRoot.yCoordinate;

        // also remember the maximum size of the whole tree
        maxWidth = 0;
        maxHeight = 0;

        treeSecondWalk(rootNode);
    }



    // Note: when this method is called, the layout must already be done 
    // (or, latest be calculated in this method, since we need the overall width of
    // the layout)

    @Override
    protected double computePrefWidth(double height) {
        if (flag) {
            doCreateLayout();
            flag = false;
        }
        System.err.printf("computePrefWidth() = %s%n", maxWidth);

        return maxWidth;
    }

    @Override
    protected double computePrefHeight(double width) {
        if (flag) {
            doCreateLayout();
            flag = false;
        }
        System.err.printf("computePrefHeight() = %s%n", maxHeight);

        return maxHeight;
    }


    @Override
    protected void layoutChildren() {
        System.err.printf("layoutChildren()%n");

        // Resizes the children to their preferred sizes. 
       super.layoutChildren();

        // position the children.
        if (needsLayout) {
            needsLayout = false;

         //   System.err.printf("LAYOUT (%s)%n", ((Pane) getChildren().get(0)).getWidth());

            TreeTraversal<T> tt5 = new DepthFirstTraversal<>();
            tt5.traversePreOrder(rootNode, (node) -> {
                LayoutDataImpl<T> layoutData = (LayoutDataImpl<T>) node.getLayoutData();

                layoutData.nodePanel.setLayoutX(layoutData.xCoordinate);
                layoutData.nodePanel.setLayoutY(layoutData.yCoordinate);

                TreeNode<T> parent = node.getParent();
                if (parent != null) {
                    LayoutDataImpl<T> layoutDataP = (LayoutDataImpl<T>) parent.getLayoutData();

                    // connection point in parent
                    double parentBottom = layoutDataP.yCoordinate + layoutDataP.nodePanel.getHeight();    // lower border
                    double parentLeft = layoutDataP.xCoordinate;
                    double parentWidth = layoutDataP.getWidth();
                    double space = parentWidth / parent.getChildren().size();
                    int i = parent.getChildren().indexOf(node);

                    layoutData.edgeLine.setStartX(parentLeft + space/2  +  i * space);
                                               // layoutDataP.xCoordinate + layoutDataP.nodePanel.getWidth() / 2.0);
                    layoutData.edgeLine.setStartY(parentBottom);

                    // connection point in child
                    layoutData.edgeLine.setEndX(layoutData.xCoordinate + layoutData.nodePanel.getWidth() / 2.0);
                    layoutData.edgeLine.setEndY(layoutData.yCoordinate);
                }

            });

        }
    }

    @Override
    public void requestLayout() {
        // System.err.printf("requestLayout()%n");
        //new Throwable().printStackTrace(System.err);
        super.requestLayout();
    }


    public void doLayout() {
        maxWidth = 0;
        maxHeight = 0;
        xTopAdjustment = 0;
        yTopAdjustment = 0;
        flModsum = 0;
        needsLayout = true;
        leftNeighbor = new HashMap<>();

        getChildren().clear();
        doCreateLayout();
    }
}
