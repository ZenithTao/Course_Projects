In this homework exercise, we compare three different
implementations of the union-find data structure:

  QuickUnionNF          -- quick union, with no weigts, no path compression
  WeightedQuickUnionUF  -- quick union with weights but no path compression
  PathCompQuickUnionUF  -- quick union with path compression but no weights
  
We will see they have roughly the same time performance,
and both are much better than plain QuickUnionUF.   Since the
second uses less space (about half), it may be preferable.

Students are expected to finish the TODO items in the three
implementations, and then package up their working solution
as a runnable jar file (sol.jar).  See Notes.txt for more
instructions on that.