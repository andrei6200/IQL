SELECT P.firstName, P.lastName, JPEG(Coll)
	INTO sqlTable
FROM Persons AS P, PhotoCollection AS Coll
