//! ステップ実行関連

#pragma once

struct HSP3DEBUG;
class KnowbugStepController;

// 実行モードを表す。HSPDEBUG_RUN_* と同じ。
enum class StepMode {
	Run,
	Stop,
	StepIn,
};

// ステップ実行の操作の種類を表す。
enum class StepControlKind {
	// 実行
	Run,

	// 停止
	Stop,

	// ステップイン (次行)
	StepIn,

	// ステップオーバー (次飛)
	StepOver,

	// ステップアウト (脱出)
	StepOut,

	// 「この呼び出しから脱出する」
	StepReturn,
};

// ステップ実行の操作を表す。
class StepControl {
	StepControlKind kind_;
	int sublev_;

	StepControl(StepControlKind kind, int sublev)
		: kind_(kind)
		, sublev_(sublev)
	{
	}

public:
	static auto run() -> StepControl {
		return StepControl{ StepControlKind::Run, 0 };
	}

	static auto stop() -> StepControl {
		return StepControl{ StepControlKind::Stop, 0 };
	}

	static auto step_in() -> StepControl {
		return StepControl{ StepControlKind::StepIn, 0 };
	}

	static auto step_over() -> StepControl {
		return StepControl{ StepControlKind::StepOver, 0 };
	}

	static auto step_out() -> StepControl {
		return StepControl{ StepControlKind::StepOut, 0 };
	}

	static auto step_return(int sublev) -> StepControl {
		return StepControl{ StepControlKind::StepReturn, sublev };
	}

	auto kind() const -> StepControlKind {
		return kind_;
	}

	auto sublev() const -> int {
		return sublev_;
	}
};

// ステップ操作を受け取ってステップ実行モードを制御する機能を担当する。
class StepController {
	// ステップ実行中かどうかのフラグ
	// 「脱出」等の条件付き実行は除く。
	bool bStepRunning;

	// 条件付き実行の終了条件となる sublev
	// (条件付き実行ではないときは -1)
	int sublevOfGoal;

	// 現在のサブルーチンレベル
	int sublev;

	// 現在の実行モード
	StepMode mode_;
	bool mode_dirty_;

public:
	StepController()
		: bStepRunning(false)
		, sublevOfGoal(-1)
		, sublev(0)
		, mode_(StepMode::Run)
		, mode_dirty_(false)
	{
	}

	void update(int sublev) {
		this->sublev = sublev;
		mode_dirty_ = false;
	}

	bool isStepRunning() const {
		return bStepRunning;
	}

	auto mode() const -> StepMode {
		return mode_;
	}

	auto is_mode_changed() const -> bool {
		return mode_dirty_;
	}

	void runStop() {
		setStepMode(StepMode::Stop);
	}

	void run() {
		setStepMode(StepMode::Run);
		bStepRunning = false;
	}

	void runStepIn() {
		// 本当のステップ実行でのみフラグが立つ
		bStepRunning = true;

		setStepMode(StepMode::StepIn);
	}

	void runStepOver() {
		return runStepReturn(sublev);
	}

	void runStepOut() {
		return runStepReturn(sublev - 1);
	}

	// ctx->sublev == sublev になるまで step を繰り返す
	void runStepReturn(int sublev) {
		if (sublev < 0) return run();

		sublevOfGoal = sublev;
		bStepRunning = false;
		setStepMode(StepMode::StepIn);
	}

	// 条件付き実行が継続されるか？
	bool continueConditionalRun() {
		if (sublevOfGoal >= 0) {
			if (sublev > sublevOfGoal) {
				setStepMode(StepMode::StepIn); // stepin を繰り返す
				return true;
			}
			else {
				sublevOfGoal = -1;
			}
		}
		return false;
	}

private:
	void setStepMode(StepMode mode) {
		mode_ = mode;
		mode_dirty_ = true;
	}
};

class KnowbugStepController {
	HSP3DEBUG* debug_;

	StepController step_controller_;

public:
	explicit KnowbugStepController(HSP3DEBUG* debug);

	bool continue_step_running();

	void update(StepControl step_control);

private:
	void begin_update();
	void end_update();
};