package com.example.tree;

import java.util.HashMap;
import java.util.Map;

import javafx.scene.layout.Region;
import javafx.scene.shape.Line;

// https://blogs.oracle.com/jfxprg/entry/dynamical_layouts_in_fx
public class TreeLayout<T> extends Region {

    private final TreeNode<T> rootNode;
    private static final float HORIZONTAL_SPACING = 5.0F;

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
    public TreeLayout(TreeNode<T> rootNode) {
        this.rootNode = rootNode;
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
            layoutData.createNodePanel(node);
            getChildren().add(layoutData.nodePanel);

            // create the edge to the parent
            TreeNode<T> parent = node.getParent();
            if (parent != null) {
                layoutData.edgeLine = new Line();
                getChildren().add(layoutData.edgeLine);
            }

        });

        System.err.println("SIZE:" + getChildren().size());

        this.applyCss();        // !!!!! Required in order to consider CSS layout properties!!!!
        calculateLayout();

        System.err.println("SIZE:" + getChildren().size());
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

    private float TreeMeanNodeSize(LayoutData<T> pLeftNode, LayoutData<T> pRightNode) {
        float result = (float) ((LayoutDataImpl<T>) pLeftNode).nodePanel.prefWidth(-1);       // !!!!!!!!! seems ok
        // System.err.println("TreeMeanNodeSize() = " + result);
        return result;      // need to consider spacing also!!
    }

    private void TreeFirstWalk(TreeNode<T> pThisNode) {
        LayoutDataImpl<T> pThisNodeLayout = (LayoutDataImpl<T>) pThisNode.getLayoutData();

        pThisNodeLayout.prev = GetPrevNodeAtLevel(pThisNode.getLevel());
        SetPrevNodeAtLevel(pThisNode.getLevel(), pThisNode);

        // pThisNode->prev = GetPrevNodeAtLevel(pThisNode.getLevel());

        // System.err.println(" => " + pThisNode);

        /* Clean up old values in a node's flModifier */
        pThisNodeLayout.flModifier = (float) 0.0;

        if (pThisNode.isLeaf()) { // || (nCurrentLevel == MAXIMUM_DEPTH)

            TreeNode<T> leftSibling = pThisNode.getLeftSibling();
            if (leftSibling != null) {
                LayoutDataImpl<T> leftSiblingLayout = (LayoutDataImpl<T>) leftSibling.getLayoutData();

                float flMeanWidth = TreeMeanNodeSize(leftSiblingLayout, pThisNodeLayout);
                pThisNodeLayout.flPrelim = leftSiblingLayout.flPrelim + HORIZONTAL_SPACING + flMeanWidth;
            } else {
                pThisNodeLayout.flPrelim = 0; /*
                                               * no sibling on the left to worry
                                               * about
                                               */
            }

        } else {
            TreeNode<T> pLeftmost = pThisNode.getFirstChild();
            TreeNode<T> pRightmost = pLeftmost; // pThisNode.getLastChild();

            TreeFirstWalk(pLeftmost);

            while (pRightmost.getRightSibling() != null) {
                pRightmost = pRightmost.getRightSibling();
                TreeFirstWalk(pRightmost);
            }

            LayoutDataImpl<T> pLeftmostLayout = (LayoutDataImpl<T>) pLeftmost.getLayoutData();
            LayoutDataImpl<T> pRightmostLayout = (LayoutDataImpl<T>) pRightmost.getLayoutData();

            float flMidpoint = (pLeftmostLayout.flPrelim + pRightmostLayout.flPrelim) / 2;
            float flMeanWidth = TreeMeanNodeSize(pLeftmostLayout, pRightmostLayout);

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
            float flMeanWidth = TreeMeanNodeSize(pLeftmost.getLayoutData(), pNeighbor.getLayoutData());

            flDistance = (((LayoutDataImpl<T>) pNeighbor.getLayoutData()).flPrelim + flLeftModsum +  80.0F + // SUBTREE_SEPARATION
                                                                                      // +
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


    private void TreeSecondWalk(TreeNode<T> pThisNode2) {
        

        // finally set the node and edge positions
        LayoutDataImpl<T> pThisNode2Layout = (LayoutDataImpl<T>) pThisNode2.getLayoutData();
        int nCurrentLevel = pThisNode2.getLevel();

        float lxTemp, lyTemp; /* hold calculations here */
        float flNewModsum; /* local modifier value */

        // // if (nCurrentLevel <= MAXIMUM_DEPTH) {
        flNewModsum = flModsum;

        lxTemp = (long) xTopAdjustment + (long) (pThisNode2Layout.flPrelim + flModsum);
        lyTemp = (long) yTopAdjustment + (long) (nCurrentLevel * 80); // LEVEL_SEPARATION);

        //
        // if (CheckExtentsRange(lxTemp, lyTemp)) {
        /* The values are within the allowable range */
        pThisNode2Layout.xCoordinate = lxTemp;
        pThisNode2Layout.yCoordinate = (int) lyTemp;

        maxWidth = Math.max(maxWidth, lxTemp + pThisNode2Layout.nodePanel.prefWidth(-1));
        maxHeight = Math.max(maxHeight, lyTemp +pThisNode2Layout.nodePanel.prefHeight(-1));

        if (!pThisNode2.isLeaf()) {
            /* Apply the flModifier value for this */
            /* node to all its offspring. */
            flModsum = flNewModsum = flNewModsum + pThisNode2Layout.flModifier;
            TreeSecondWalk(pThisNode2.getFirstChild());
            flNewModsum = flNewModsum - pThisNode2Layout.flModifier;
        }

        TreeNode<T> rightSibling = pThisNode2.getRightSibling();
        if (rightSibling != null) {
            flModsum = flNewModsum;
            TreeSecondWalk(rightSibling);
        }
    }

    
    private void calculateLayout() {
        TreeFirstWalk(rootNode);

        LayoutDataImpl<T> ldRoot = (LayoutDataImpl<T>) rootNode.getLayoutData();
        xTopAdjustment = ldRoot.xCoordinate;
        yTopAdjustment = ldRoot.yCoordinate;

        // also remember the maximum size of the whole tree
        maxWidth = 0;
        maxHeight = 0;

        TreeSecondWalk(rootNode);
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
        double result = maxWidth;
        System.err.println("computePrefWidth() = " + result);
        return maxWidth; // super.computePrefWidth(height);
    }

    @Override
    protected double computePrefHeight(double width) {
        System.err.println("PREFHEIGHT:" + maxHeight);
        return maxHeight; // super.computePrefHeight(width);
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
