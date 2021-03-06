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

#include "gui/tuna_gui.hpp"
#include "query/vlc_obs_source.hpp"
#include "util/config.hpp"
#include "util/constants.hpp"
#include "util/utility.hpp"
#include <QAction>
#include <QMainWindow>
#include <obs-frontend-api.h>
#include <obs-module.h>

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE(S_PLUGIN_ID, "en-US")

void register_gui()
{
    /* UI registration from
     * https://github.com/Palakis/obs-websocket/
     */
    const auto menu_action = static_cast<QAction*>(obs_frontend_add_tools_menu_qaction(T_MENU_TUNA));
    obs_frontend_push_ui_translation(obs_module_get_string);
    const auto main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
    tuna_dialog = new tuna_gui(main_window);
    obs_frontend_pop_ui_translation();
    const auto menu_cb = [] { tuna_dialog->toggleShowHide(); };
    QAction::connect(menu_action, &QAction::triggered, menu_cb);
}

bool obs_module_load()
{
    util::load_vlc();
    config::load();
    register_gui();
    return true;
}

void obs_module_unload()
{
    config::close();
    util::unload_vlc();
}
