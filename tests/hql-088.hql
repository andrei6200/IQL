	SELECT P.firstName, P.lastName, JPEG(Coll)
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, JPEG(Coll)
	FROM Ratings AS R, PhotoCollection AS Coll
