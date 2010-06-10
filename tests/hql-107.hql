SELECT P.firstName, P.lastName, 
	MARRAY x IN [1:100],
	       y IN [20:40]
	VALUES Coll[x,y]

	INTO sqlTable
FROM Persons AS P, PhotoCollection AS Coll
