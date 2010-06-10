SELECT P.firstName, P.lastName, 255 - Coll.red
	INTO sqlTable
FROM Persons AS P, PhotoCollection AS Coll
