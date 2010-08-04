SELECT ST_Area(ST_Union(the_geom)) FROM towns WHERE town = 'BOSTON';
