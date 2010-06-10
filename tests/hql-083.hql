	SELECT P.firstName, P.lastName, Coll [1:100, 200:500]
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, Coll [101:201, 200:500]
	FROM Ratings AS R, PhotoCollection AS Coll
