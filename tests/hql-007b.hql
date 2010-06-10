SELECT P.firstName, P.lastName, 
	MARRAY x IN [ L.lower1 : L.upper1 ],
	       y IN [ L.lower2 : L.upper2 ]
	VALUES Coll[x,y] * P.size
FROM (Persons AS P NATURAL JOIN IntegerLimits AS L),
	PhotoCollection AS Coll
