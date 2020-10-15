// Type your code here, or load an example.

#include <utility>
#include <Windows.h>
#include <bitset>
#include <thread>
#include <chrono>
#include <iostream>

class KeyManager
{
public:
	using VK_TABLE_DATA_SIZE = unsigned char;
	static constexpr uint8_t KEY_STATE_BIT_SIZE = 17 /*unsigned short bit */;
	static constexpr VK_TABLE_DATA_SIZE KeyTableSize = (std::numeric_limits<VK_TABLE_DATA_SIZE>::max)();
	using KeyTableType = std::bitset<KeyTableSize>;
public:
	KeyManager() 
	{
		CurrentFrameKeyPressTable.reset();
		CurrentFrameKeyDownTable.reset();
		CurrentFrameKeyUpTable.reset();
	}
	~KeyManager() {};
public:
	// 키 입력을 처리 받기 전의 루프의 최상단에서 호출을 권장.
	void Key_Update()
	{
		CurrentFrameKeyPressTable.reset();
		// 1 ~ 256 윈도우 VK 테이블 사이즈 만큼
		// 키 테이블을 늘린다면 오버플로우를 유의 ( 그럴일은 없겠지만 ) 
		for (VK_TABLE_DATA_SIZE CurrentVK_TABLE_KEY = 0x01; CurrentVK_TABLE_KEY <= 0xFE; ++CurrentVK_TABLE_KEY)
		{
			if (GetAsyncKeyState(CurrentVK_TABLE_KEY) & 0x8000)
			{
				CurrentFrameKeyPressTable.set(CurrentVK_TABLE_KEY);
			}
		}
	}
	//				0010
	bool Key_Up(VK_TABLE_DATA_SIZE VK_TABLE_KEY)
	{
		// 0011 & 0010 = 0010
		if (CurrentFrameKeyPressTable.test(VK_TABLE_KEY))
		{
			//0000 | 0010 = 0010

			CurrentFrameKeyUpTable.set(VK_TABLE_KEY);
			return false;
		}
		// 0111 1000 0000
		else if (CurrentFrameKeyUpTable.test(VK_TABLE_KEY))
		{
			// 0111 ^ 0010 = 0101
			CurrentFrameKeyUpTable.reset(VK_TABLE_KEY);
			return true;
		}
		return false;
	}
	bool Key_Down(VK_TABLE_DATA_SIZE VK_TABLE_KEY)
	{
		if ((CurrentFrameKeyPressTable.test(VK_TABLE_KEY)) && !(CurrentFrameKeyDownTable.test(VK_TABLE_KEY)))
		{
			CurrentFrameKeyDownTable.set(VK_TABLE_KEY);
			return true;
		}
		else if (!(CurrentFrameKeyPressTable.test(VK_TABLE_KEY)) && (CurrentFrameKeyDownTable.test(VK_TABLE_KEY)))
		{
			CurrentFrameKeyDownTable.reset(VK_TABLE_KEY);
			return false;
		}
		return false;
	}
	bool Key_Pressing(VK_TABLE_DATA_SIZE VK_TABLE_KEY)
	{
		if (CurrentFrameKeyPressTable.test(VK_TABLE_KEY))
			return true;

		return false;
	}
private:
	KeyTableType  CurrentFrameKeyPressTable;
	KeyTableType CurrentFrameKeyDownTable;
	KeyTableType CurrentFrameKeyUpTable;
};

#include <iostream>

int main()
{
	KeyManager _KeyManager;

	while (1)
	{
		_KeyManager.Key_Update();

		if (_KeyManager.Key_Down(VK_RBUTTON))
		{
			std::cout << "DOWN LEFT" << std::endl;
		}
		if (_KeyManager.Key_Pressing(VK_RBUTTON))
		{
			std::cout << "PRESS LEFT" << std::endl;
		}
		if (_KeyManager.Key_Up(VK_RBUTTON))
		{
			std::cout << "UP LEFT" << std::endl;
		}
	};
};
