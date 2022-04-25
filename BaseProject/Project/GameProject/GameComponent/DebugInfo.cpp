#include "DebugInfo.h"

DebugInfo* DebugInfo::GetInstance()
{
	if (!m_instance) {

		m_instance = new DebugInfo;
		return m_instance;
	}
	return m_instance;
}

DebugInfo::DebugInfo() : Base(UpdatePriority::eUp_HUD,"DebugInfo"),m_debug_text("‚l‚rƒSƒVƒbƒN",24)
{
	// •`‰æƒ^ƒXƒN‚É“o˜^
	m_Render.RegistRender(this, (RenderFunc)&DebugInfo::Render, RenderPriority::eRd_Player, "Player");
}

void DebugInfo::Update()
{

}

void DebugInfo::Render()
{
	m_debug_text.Draw(100,100,1,1,1,"‚ ‚ ‚ ‚ ");

}

void DebugInfo::Draw(float x, float y, float r, float g, float b, const char* format,...)
{
	char buf[256];
	va_list ap;
	int length = 0;

	if (format == NULL)
	{
		return;
	}

	//•¶Žš—ñ•ÏŠ· 
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);
	m_debug_text.Draw(x, y, r, g, b, format);
}
