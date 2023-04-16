#include "Player.h"
#include "RenderSystem.h"
#include "Global.h"
#include "InputSystem.h"

void Player::Reset()
{
	// �÷��̾� ���� ��ġ ����
	COORD PlayerStart;
	PlayerStart.X = global::playerMovableRect.Left + (global::playerMovableRect.Left + global::playerMovableRect.Right) / 2;
	PlayerStart.Y = global::playerMovableRect.Bottom - 2;
	SetPosition(PlayerStart);
	
	Score =0;
}

void Player::SetPosition(COORD value)
{
	cPos = value;
	fPosX = (float)cPos.X;
	fPosY = (float)cPos.Y;
}


void Player::Update()
{
	// Ű �Է°� ȭ�� ��°� ���� ���� ������ �и��ؼ� ������ ���ô�.
	static ULONGLONG elapsedTime;

	elapsedTime += global::time::GetDeltaTime();

	while (elapsedTime >= playerMoveSpeed)
	{
		UpdatePlayerPosition();

		elapsedTime -= playerMoveSpeed;
	}
}


void Player::UpdatePlayerPosition()
{
	if (global::input::IsEscapeCmdOn())
	{
		global::input::Set(global::input::ESCAPE_KEY_INDEX, false);

		//TODO: ���� ȭ�� ��ȯ�̳� ���� ó��

		return; // �ٸ� Ű �Է��� �Ű� �� �ʿ䰡 �����.
	}

	if (global::input::IsLeftCmdOn())
	{
		global::input::Set(global::input::USER_CMD_LEFT, false);

		cPos.X--;
		Clamp(cPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
	}

	if (global::input::IsRightCmdOn())
	{
		global::input::Set(global::input::USER_CMD_RIGHT, false);

		cPos.X++;
		Clamp(cPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
	}
	if (global::input::IsUpCmdOn())
	{
		global::input::Set(global::input::USER_CMD_UP, false);

		cPos.Y--;
		Clamp(cPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
	}

	if (global::input::IsDownCmdOn())
	{
		global::input::Set(global::input::USER_CMD_DOWN, false);

		cPos.Y++;
		Clamp(cPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
	}

	if (global::input::IsSpaceCmdOn())
	{
		global::input::Set(global::input::USER_CMD_SPACE, false);

		SpawnBullet();
	}

	fPosX = (float)cPos.X;
	fPosY = (float)cPos.Y;
}


void Player::Draw()
{
	render::ScreenDraw(cPos.X, cPos.Y, 'P');
}

void Player::SpawnBullet()
{
	static ULONGLONG elapsedTime;

	elapsedTime += global::time::GetDeltaTime();
	if (elapsedTime >= BulletInterval)
	{
		elapsedTime-= BulletInterval;
		Bullet Info;
		Info.SetPosition(cPos);

		COORD target;
		target.X = cPos.X;
		target.Y = cPos.Y - 1;
		Info.SetDiection(target);
		Info.SetPlayerBullet();
		global::PlayerBulletContainer.InsertAtHead(Info);
	}
}
