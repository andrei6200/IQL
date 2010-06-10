SELECT P.lastName, LEN(P.lastName), 
	MARRAY x IN [10:110],
	       y IN [40:60]
	VALUES Coll[x,y]
FROM Persons AS P, PhotoCollection AS Coll
