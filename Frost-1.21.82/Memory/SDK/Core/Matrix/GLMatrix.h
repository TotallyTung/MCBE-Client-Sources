#pragma once

struct GLMatrix {
public:
	float matrix[16];
	float matrix_nest[4][4];

	GLMatrix* correct() {
		GLMatrix* newMatPtr = new GLMatrix;

		for (int i = 0; i < 4; i++) {
			newMatPtr->matrix[i * 4 + 0] = matrix[0 + i];
			newMatPtr->matrix[i * 4 + 1] = matrix[4 + i];
			newMatPtr->matrix[i * 4 + 2] = matrix[8 + i];
			newMatPtr->matrix[i * 4 + 3] = matrix[12 + i];
		}
		return newMatPtr;
	};

	bool OWorldToScreen(Vector3<float> origin, Vector3<float> pos, Vector2<float>& screen, Vector2<float>& fov, Vector2<float>& displaySize) const
	{
		pos = pos.submissive(origin);

		/*float x = transformDot(0, pos);
		float y = transformDot(1, pos);
		float z = transformDot(2, pos);*/
		
		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0)
			return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	float transformDot(int index, const Vector3<float>& p) const {
		return p.x * matrix[index * 4 + 0] +
			p.y * matrix[index * 4 + 1] +
			p.z * matrix[index * 4 + 2] +
			matrix[index * 4 + 3];
	}

	__forceinline float transformx(const Vector3<float>& p) const {
		return p.x * matrix[0] + p.y * matrix[4] + p.z * matrix[8] + matrix[12];
	}

	__forceinline float transformy(const Vector3<float>& p) const {
		return p.x * matrix[1] + p.y * matrix[5] + p.z * matrix[9] + matrix[13];
	}

	__forceinline float transformz(const Vector3<float>& p) const {
		return p.x * matrix[2] + p.y * matrix[6] + p.z * matrix[10] + matrix[14];
	}

	struct Vector3Hasher {
		std::size_t operator()(const Vector3<int>& k) const {
			using std::size_t;
			using std::hash;
			return ((hash<int>()(k.x)
				^ (hash<int>()(k.y) << 1)) >> 1)
				^ (hash<int>()(k.z) << 1);
		}
	};
};