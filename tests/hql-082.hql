	SELECT P.firstName, P.lastName, Coll
	FROM Persons AS P, PhotoCollection AS Coll
	WHERE avg_cells(Coll) > 128
EXCEPT
	SELECT R.firstName, R.lastName, Coll
	FROM Ratings AS R, PhotoCollection AS Coll
	WHERE add_cells(Coll) < 100
