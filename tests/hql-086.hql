	SELECT P.firstName, P.lastName, 
		CONDENSE +
		OVER i IN [1:100, 1:100]
		USING Coll[i]
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, avg_cells(Coll)
	FROM Ratings AS R, PhotoCollection AS Coll
