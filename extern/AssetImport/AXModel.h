class AXModel
{
public:
	int numVerts;
	vec3 *positions;
	vec3 *normals;
	vec2 *textureUVs;
	int triCount;
	vec3i *triangles;

	AXModel(const char *modelFileName)
	{
		FILE *modelFile = NULL;
		int version;
		
		char objectName[256];


		modelFile = fopen( modelFileName, "r");

		if ( !modelFile )
		{
			printf("Error opening %s", modelFileName);
			exit(1);
		}

		fscanf( modelFile, "// %*[^\n]\n");
		fscanf( modelFile, "// %*[^\n]\n");
		fscanf( modelFile, "// %*[^\n]\n");



		fscanf( modelFile, "MODEL\nVERSION %d\n", &version);
		fscanf( modelFile, "Object %s\n", &objectName);

		fscanf( modelFile, "NUMVERTS %d\n", &numVerts);

		positions = new vec3[numVerts];
		normals = new vec3[numVerts];
		textureUVs = new vec2[numVerts];

		for ( int i = 0; i < numVerts; ++i )
		{
			fscanf( modelFile, "VERT %*d\n" );
			fscanf( modelFile, "Position %f %f %f\n", &(positions[i].x), &(positions[i].y), &(positions[i].z) );
			fscanf( modelFile, "NORMAL %f %f %f\n", &(normals[i].x), &(normals[i].y), &(normals[i].z) );
			fscanf( modelFile, "UV %f %f\n", &(textureUVs[i].x), &(textureUVs[i].y) );
		}

		fscanf( modelFile, "Index List : Triangle Count %d", &triCount);

		triangles = new vec3i[triCount];

		for ( int i = 0; i < triCount; ++i )
		{
			fscanf( modelFile, "%d %d %d\n", &(triangles[i].x), &(triangles[i].y), &(triangles[i].z));
		}

		fclose( modelFile );
	}

};
