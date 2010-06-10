	SELECT P.firstName, P.lastName, Coll
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, Coll
	FROM Ratings AS R, PhotoCollection AS Coll
