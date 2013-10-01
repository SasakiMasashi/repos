#pragma once

namespace GPS
{
	class ExecutionManager
	{
	public:
		enum ExecutionMode
		{
			Non,
			BatFileMode,
			ApplyMode
		};

	public:
		ExecutionManager() : m_ExecutionMode(ApplyMode) {}

		void				SetExecutionMode(const ExecutionMode mode) { m_ExecutionMode = mode; }
		ExecutionMode		GetExecutionMode() const { return m_ExecutionMode; }

	private:
		ExecutionMode m_ExecutionMode;
	};
}