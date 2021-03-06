/*************************************************************************
 * This file is part of tuna
 * github.con/univrsal/tuna
 * Copyright 2019 univrsal <universailp@web.de>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "window_source.hpp"
#include "../gui/tuna_gui.hpp"
#include "../util/config.hpp"
#include "../util/utility.hpp"
#include "../util/window/window_helper.hpp"
#include <QRegularExpression>

window_source::window_source()
{
    m_capabilities = CAP_TITLE;
}

void window_source::load()
{
    CDEF_STR(CFG_WINDOW_TITLE, "");
    CDEF_BOOL(CFG_WINDOW_REGEX, false);
    CDEF_STR(CFG_WINDOW_SEARCH, "");
    CDEF_STR(CFG_WINDOW_REPLACE, "");
    CDEF_STR(CFG_WINDOW_PAUSE, "");
    CDEF_UINT(CFG_WINDOW_CUT_BEGIN, 0);
    CDEF_UINT(CFG_WINDOW_CUT_END, 0);

    m_title = CGET_STR(CFG_WINDOW_TITLE);
    m_regex = CGET_BOOL(CFG_WINDOW_REGEX);
    m_search = CGET_STR(CFG_WINDOW_SEARCH);
    m_replace = CGET_STR(CFG_WINDOW_REPLACE);
    m_pause = CGET_STR(CFG_WINDOW_PAUSE);
    m_cut_begin = CGET_UINT(CFG_WINDOW_CUT_BEGIN);
    m_cut_end = CGET_UINT(CFG_WINDOW_CUT_END);
}

void window_source::save()
{
    CSET_STR(CFG_WINDOW_TITLE, m_title.c_str());
    CSET_STR(CFG_WINDOW_SEARCH, m_search.c_str());
    CSET_STR(CFG_WINDOW_REPLACE, m_replace.c_str());
    CSET_STR(CFG_WINDOW_PAUSE, m_pause.c_str());
    CSET_BOOL(CFG_WINDOW_REGEX, m_regex);
    CSET_UINT(CFG_WINDOW_CUT_BEGIN, m_cut_begin);
    CSET_UINT(CFG_WINDOW_CUT_END, m_cut_begin);
}

void window_source::refresh()
{
    if (m_title.empty())
        return;

    std::vector<std::string> window_titles;
    GetWindowList(window_titles);
    QRegularExpression regex(m_title.c_str());
    std::string result = "";

    for (const auto& title : window_titles) {
        bool matches = false;
        if (m_regex) {
            matches = regex.match(title.c_str()).hasMatch();
        } else {
            /* Direct search */
            matches = title.find(m_title) != std::string::npos;
            if (matches && !m_pause.empty() && title.find(m_pause) == std::string::npos)
                matches = false;
        }

        if (matches) {
            result = title;
            break;
        }
    }

    m_current = {};
    if (result.empty()) {
        m_current.is_playing = false;
        m_current.data = CAP_STATUS;
    } else {
        /* Replace & cut */
        util::replace_all(result, m_search, m_replace);
        if (0 < m_cut_end + m_cut_begin && m_cut_end + m_cut_begin < result.length())
            result = result.substr(m_cut_begin, result.length() - m_cut_begin - m_cut_end);

        m_current.data = CAP_TITLE | CAP_STATUS;
        m_current.is_playing = true;
        m_current.title = result;
    }
}

bool window_source::execute_capability(capability c)
{
    /* NO-OP */
    return true;
}

void window_source::load_gui_values()
{
    tuna_dialog->set_window_regex(m_regex);
    tuna_dialog->set_window_title(m_title.c_str());
    tuna_dialog->set_window_search(m_search.c_str());
    tuna_dialog->set_window_replace(m_replace.c_str());
    tuna_dialog->set_window_pause(m_pause.c_str());
    tuna_dialog->set_window_cut_begin(m_cut_begin);
    tuna_dialog->set_window_cut_end(m_cut_end);
}
