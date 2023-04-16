#include <windows.h>
#include <cassert>
#include "InputSystem.h"


namespace global
{
	namespace input
	{
		bool inputKeyTable[MAX_KEY]; // ���ǵ� Ű ���� ó�� �մϴ�.

		// Helper Functions ..
		void Set(const int keyIdx, bool bOn) 
		{	
			assert(keyIdx >= 0 && keyIdx < MAX_KEY); // ��� �ڵ带 �ۼ��ϴ� �Ϳ� �ͼ������� �մϴ�.  �迭�� �ٷ� ���� �ε����� �����ؾ� �մϴ�!!

			inputKeyTable[keyIdx] = bOn;			// bool Ÿ���� Ȱ�� �սô�.
		}

		bool IsEscapeCmdOn()
		{
			return inputKeyTable[ESCAPE_KEY_INDEX];
		}

		bool IsLeftCmdOn()
		{
			return inputKeyTable[USER_CMD_LEFT];
		}

		bool IsRightCmdOn()
		{
			return inputKeyTable[USER_CMD_RIGHT];
		}

		bool IsUpCmdOn()
		{
			return inputKeyTable[USER_CMD_UP];
		}

		bool IsDownCmdOn()
		{
			return inputKeyTable[USER_CMD_DOWN];
		}

		bool IsSpaceCmdOn()
		{
			return inputKeyTable[USER_CMD_SPACE];
		}


		// �Է� Ű���� ���� �˾� ���ô�. ��ŷ�� �ͺ�ŷ�� ���ؼ��� �����ϰ� ����� �մϴ�!!
		// * ��Ʈ ���꿡 ���� �н��մϴ�. & �� && �� �޶��. 
		// &, * �����ڰ� �ƶ��� ���� �ٸ��� �ؼ��Ǵ� �Ϳ� ���� �ؾ� �մϴ�. 
		void UpdateInput()
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, true);
			}
			
			if (GetAsyncKeyState('A') & 0x8000) //���� 'A'
			{
				global::input::Set(USER_CMD_LEFT, true);
			}
			
			if (GetAsyncKeyState('D') & 0x8000) //������ 'D'
			{
				global::input::Set(USER_CMD_RIGHT, true);
			}
			if (GetAsyncKeyState('W') & 0x8000) //�� 'W'
			{
				global::input::Set(USER_CMD_UP, true);
			}

			if (GetAsyncKeyState('S') & 0x8000) //�Ʒ� 'S'
			{
				global::input::Set(USER_CMD_DOWN, true);
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) //SPACE
			{
				global::input::Set(USER_CMD_SPACE, true);
			}

		}
	};
};



