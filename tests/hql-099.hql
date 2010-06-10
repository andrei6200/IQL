SELECT P.lastName, LEN(P.lastName), 
	sdom(Coll), oid(Coll)
FROM Persons AS P, PhotoCollection AS Coll
