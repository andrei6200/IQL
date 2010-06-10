	SELECT P.firstName, P.lastName, 
		MARRAY x IN [1:100],
		       y IN [20:40]
		VALUES Coll[x,y]
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, Coll
	FROM Ratings AS R, PhotoCollection AS Coll
