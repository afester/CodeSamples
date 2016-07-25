package com.example.tree;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

import javafx.scene.Node;
import javafx.scene.layout.Region;
import javafx.scene.layout.Pane;
import javafx.scene.shape.Line;

// https://blogs.oracle.com/jfxprg/entry/dynamical_layouts_in_fx
public class TreeLayout<T> extends Region {

    private final TreeNode<T> rootNode;
    private final Function<TreeNode<T>, Node> createNode;
    private final Function<TreeNode<T>, Node> createEdge;

    private static final float HORIZONTAL_SPACING = 5.0F;
    private static final float SUBTREE_SEPARATION = 80.0F;
    private static final int LEVEL_SEPARATION = 120;

    private boolean needsLayout = true;
    private double maxWidth = 0;
    private double maxHeight = 0;

    private float xTopAdjustment;
    private float yTopAdjustment;

    private boolean flag = true;

    // maintain a list of neighboring nodes on each level
    // this seems to maintain the left neighbors of the current sub tree only
    private Map<Integer, TreeNode<T>> leftNeighbor = new HashMap<>();

    private float flModsum = (float) 0.0;


    /**
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
        TreeTraversal<T> tt5 = new DepthFirstTraversal<>();
        tt5.traversePreOrder(rootNode, (node) -> {
            // initialize layout data for this node.
            LayoutDataImpl<T> layoutData = new LayoutDataImpl<>();
            node.setLayoutData(layoutData);

            // create the node
            Node nodeView = createNode.apply(node);
            layoutData.nodePanel = (Pane) nodeView;
            getChildren().add(nodeView);

            // create the edge to the parent
            TreeNode<T> parent = node.getParent();
            if (parent != null) {
                Node edgeView = createEdge.apply(node);
                layoutData.edgeLine = (Line) edgeView;
                getChildren().add(edgeView);
            }
        });


        this.applyCss();        // !!!!! Required in order to consider CSS layout properties!!!!
        calculateLayout();
        
        TreeTraversal<T> dump = new DepthFirstTraversal<>();
        dump.traversePreOrder(rootNode, (node) -> {
            System.err.println(node + " => " + node.getLayoutData());
        });
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
        System.err.printf("%s / %s%n", pLeftNode, pRightNode);
        float result = 0;

        if (pLeftNode != null) {
            LayoutDataImpl<T> pLeftNodeLayout = (LayoutDataImpl<T>) pLeftNode.getLayoutData();
            result = (float) (pLeftNodeLayout.nodePanel.prefWidth(-1)) / 2;       // !!!!!!!!! seems ok
        }

        if (pRightNode != null) {
            LayoutDataImpl<T> pRightNodeLayout = (LayoutDataImpl<T>) pRightNode.getLayoutData();
            result = result + (float) pRightNodeLayout.nodePanel.prefWidth(-1) / 2;       // !!!!!!!!! seems ok
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

        pThisNodeLayout.prev = GetPrevNodeAtLevel(pThisNode.getLevel());
        SetPrevNodeAtLevel(pThisNode.getLevel(), pThisNode);

        /* Clean up old values in a node's flModifier */
        pThisNodeLayout.flModifier = (float) 0.0;

        if (pThisNode.isLeaf()) { // || (nCurrentLevel == MAXIMUM_DEPTH)

            TreeNode<T> leftSibling = pThisNode.getLeftSibling();
            if (leftSibling != null) {
                LayoutDataImpl<T> leftSiblingLayout = (LayoutDataImpl<T>) leftSibling.getLayoutData();

                float flMeanWidth = treeMeanNodeSize(leftSibling, pThisNode);
                pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + HORIZONTAL_SPACING + flMeanWidth;
            } else {
                pThisNodeLayout.flPrelim = 0; // no sibling on the left to worry about
            }

        } else {    // Not a leaf node. Calculate midpoint between its children.

//            TreeNode<T> pLeftmost = pThisNode.getFirstChild();
//            TreeNode<T> pRightmost = pLeftmost;

            // walk through all child nodes
            for (TreeNode<T> child : pThisNode.getChildren()) {
                treeFirstWalk(child);
            }

            TreeNode<T> pLeftmost = pThisNode.getFirstChild();
            TreeNode<T> pRightmost = pThisNode.getLastChild();

//            treeFirstWalk(pLeftmost);
//            while (pRightmost.getRightSibling() != null) {
//                pRightmost = pRightmost.getRightSibling();
//                treeFirstWalk(pRightmost);
//            }

            LayoutDataImpl<T> pLeftmostLayout = (LayoutDataImpl<T>) pLeftmost.getLayoutData();
            LayoutDataImpl<T> pRightmostLayout = (LayoutDataImpl<T>) pRightmost.getLayoutData();

            float flMidpoint = (pLeftmostLayout.flPrelim + pRightmostLayout.flPrelim) / 2;

            System.err.printf("%s / %s => %s%n", pLeftmost, pRightmost, flMidpoint);

            float flMeanWidth = treeMeanNodeSize(pLeftmost, pThisNode);

            TreeNode<T> leftSibling = pThisNode.getLeftSibling();
            if (leftSibling != null) {
                LayoutDataImpl<T> leftSiblingLayout = (LayoutDataImpl<T>) leftSibling.getLayoutData();

                pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + HORIZONTAL_SPACING + flMeanWidth;
                pThisNodeLayout.flModifier = pThisNodeLayout.flPrelim - flMidpoint;

                TreeApportion(pThisNode); // , nCurrentLevel);
            } else {
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

    private void TreeApportion(TreeNode<T> pThisNode) {
        // TreeNode<T> pLeftmost; /* leftmost at given level*/
        // PNODE pNeighbor; /* node left of pLeftmost */
        TreeNode<T> pAncestorLeftmost; /* ancestor of pLeftmost */
        TreeNode<T> pAncestorNeighbor; /* ancestor of pNeighbor */
        TreeNode<T> pTempPtr; /* loop control pointer */
        // unsigned i; /* loop control */
        // unsigned nCompareDepth; /* depth of comparison */
        // /* within this proc */
        // unsigned nDepthToStop; /* depth to halt */
        int nLeftSiblings; /* nbr of siblings to the */
        // /* left of pThisNode, including pThisNode, */
        // /* til the ancestor of pNeighbor */
        float flLeftModsum; /* sum of ancestral mods */
        float flRightModsum; /* sum of ancestral mods */
        float flDistance; /* difference between */
        // /* where pNeighbor thinks pLeftmost should be */
        // /* and where pLeftmost actually is */
        float flPortion; /* proportion of */
        // /* flDistance to be added to each sibling */

        TreeNode<T> pLeftmost = pThisNode.getFirstChild();
        TreeNode<T> pNeighbor = ((LayoutDataImpl<T>) pLeftmost.getLayoutData()).prev; // .getLeftSibling();
                                                           // // !!!!!!!!!!!
                                                           // LeftNeighbor(pLeftmost);

        int nCompareDepth = 1;
        int nDepthToStop = /* MAXIMUM_DEPTH */ 20 - pThisNode.getLevel(); // nCurrentLevel;

        while ((pLeftmost != null) && (pNeighbor != null) && (nCompareDepth <= nDepthToStop)) {

            /* Compute the location of pLeftmost and where it */
            /* should be with respect to pNeighbor. */
            flRightModsum = flLeftModsum = (float) 0.0;
            pAncestorLeftmost = pLeftmost;
            pAncestorNeighbor = pNeighbor;
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

            flDistance = (((LayoutDataImpl<T>) pNeighbor.getLayoutData()).flPrelim + flLeftModsum +  SUBTREE_SEPARATION
                                                                                      +
                    (float) flMeanWidth) - (((LayoutDataImpl<T>)pLeftmost.getLayoutData()).flPrelim + flRightModsum);

            if (flDistance > (float) 0.0) {
                /* Count the interior sibling subtrees */
                nLeftSiblings = 0;
                for (pTempPtr = pThisNode; (pTempPtr != null)
                        && (pTempPtr != pAncestorNeighbor); pTempPtr = pTempPtr.getLeftSibling()) { // Leftsibling(pTempPtr))
                                                                                                    // {
                    nLeftSiblings++;
                }

                if (pTempPtr != null) {
                    /* Apply portions to appropriate */
                    /* leftsibling subtrees. */
                    flPortion = flDistance / (float) nLeftSiblings;
                    for (pTempPtr = pThisNode; (pTempPtr != pAncestorNeighbor); pTempPtr = pTempPtr.getLeftSibling()) {
                        ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flPrelim = ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flPrelim + flDistance;
                        ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flModifier = ((LayoutDataImpl<T>) pTempPtr.getLayoutData()).flModifier + flDistance;
                        flDistance = flDistance - flPortion;
                    }
                } else {
                    /* Don't need to move anything--it needs */
                    /* to be done by an ancestor because */
                    /* pAncestorNeighbor and */
                    /* pAncestorLeftmost are not siblings of */
                    /* each other. */
                    return;
                }
            } /* end of the while */

            /* Determine the leftmost descendant of pThisNode */
            /* at the next lower level to compare its */
            /* positioning against that of its pNeighbor. */

            nCompareDepth++;
            if (pLeftmost.isLeaf()) {
                pLeftmost = TreeGetLeftmost(pThisNode, 0, nCompareDepth);
            } else {
                pLeftmost = pLeftmost.getFirstChild();
            }

            if (pLeftmost != null) {
                pNeighbor = ((LayoutDataImpl<T>) pLeftmost.getLayoutData()).prev;
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

        float lxTemp, lyTemp;   /* hold calculations here */
        float flNewModsum;      /* local modifier value */

        // // if (nCurrentLevel <= MAXIMUM_DEPTH) {
        flNewModsum = flModsum;

        lxTemp = (long) xTopAdjustment + (long) (pThisNodeLayout.flPrelim + flModsum);
        lyTemp = (long) yTopAdjustment + (long) (nCurrentLevel * LEVEL_SEPARATION);

        //
        // if (CheckExtentsRange(lxTemp, lyTemp)) {
        /* The values are within the allowable range */
        pThisNodeLayout.xCoordinate = lxTemp;
        pThisNodeLayout.yCoordinate = (int) lyTemp;

        maxWidth = Math.max(maxWidth, lxTemp + pThisNodeLayout.nodePanel.prefWidth(-1));
        maxHeight = Math.max(maxHeight, lyTemp +pThisNodeLayout.nodePanel.prefHeight(-1));

        if (!pThisNode.isLeaf()) {
            /* Apply the flModifier value for this */
            /* node to all its offspring. */
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

        System.err.println("computePrefWidth() = " + maxWidth);
        return maxWidth;
    }

    @Override
    protected double computePrefHeight(double width) {
        if (flag) {
            doCreateLayout();
            flag = false;
        }

        System.err.println("PREFHEIGHT:" + maxHeight);
        return maxHeight;
    }


    @Override
    protected void layoutChildren() {

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

                    layoutData.edgeLine.setStartX(layoutDataP.xCoordinate + layoutDataP.nodePanel.getWidth() / 2.0);
                    layoutData.edgeLine.setStartY(layoutDataP.yCoordinate + layoutDataP.nodePanel.getHeight());
                    layoutData.edgeLine.setEndX(layoutData.xCoordinate + layoutData.nodePanel.getWidth() / 2.0);
                    layoutData.edgeLine.setEndY(layoutData.yCoordinate);
                }

            });

        }
    }


    public void doLayout() {
        maxWidth = 0;
        maxHeight = 0;
        xTopAdjustment = 0;
        yTopAdjustment = 0;
        flModsum = 0;
//        flag = true;
       needsLayout = true;
       leftNeighbor = new HashMap<>();

        getChildren().clear();
        doCreateLayout();
        
    }
}