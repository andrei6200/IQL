SELECT Coll1, Coll2
FROM PhotoCollection AS Coll1 JOIN PhotoCollection AS Coll2
	ON avg_cells(Coll1) > avg_cells(Coll2)
