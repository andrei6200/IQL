	SELECT P.firstName, P.lastName, 255 - Coll.red
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, 255 - Coll.green
	FROM Ratings AS R, PhotoCollection AS Coll
