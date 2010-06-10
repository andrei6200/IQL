SELECT P.firstName, P.lastName, R.rating, Coll
FROM PhotoCollection AS Coll JOIN Persons AS P
	ON P.oid = oid(Coll)
