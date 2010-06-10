	SELECT P.firstName, P.lastName, sdom(Coll), oid(Coll)
	FROM Persons AS P, PhotoCollection AS Coll
EXCEPT
	SELECT R.firstName, R.lastName, sdom(Coll), oid(Coll)
	FROM Ratings AS R, PhotoCollection AS Coll
