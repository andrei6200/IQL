SELECT P.firstName, P.lastName, 
	scale(Coll, [P.size, P.size+1])
FROM Persons AS P, PhotoCollection AS Coll
