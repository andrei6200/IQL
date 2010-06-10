	SELECT P.firstName, P.lastName, scale(Coll, [2,3])
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, scale(Coll, [2,3])
	FROM Ratings AS R, PhotoCollection AS Coll
