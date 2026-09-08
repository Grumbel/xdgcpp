// Copyright (C) 2015 Thomas Voß <thomas.voss.bochum@gmail.com>
//               2026 Ingo Ruhnke <grumbel@gmail.com>
//
// This library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <xdg.h>

#include <boost/test/unit_test.hpp>

#include <cstdlib>
#include <iostream>

// ---------------------------------------------------------------------------
// XDG_DATA_HOME
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(XdgDataHomeIgnoresRelativeDirectoryFromEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_DATA_HOME", "tmp", 1);
    // Relative path is invalid → ignored → fall back to default
    BOOST_CHECK_EQUAL("/tmp/.local/share", xdg::BaseDirSpecification::create()->data().home());
    BOOST_CHECK_EQUAL("/tmp/.local/share", xdg::data().home());
}

BOOST_AUTO_TEST_CASE(XdgDataHomeReturnsDefaultValueForEmptyEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_DATA_HOME", "", 1);
    BOOST_CHECK_EQUAL("/tmp/.local/share", xdg::BaseDirSpecification::create()->data().home());
    BOOST_CHECK_EQUAL("/tmp/.local/share", xdg::data().home());
}

BOOST_AUTO_TEST_CASE(XdgDataHomeUsesAbsoluteEnvValue)
{
    ::setenv("XDG_DATA_HOME", "/custom/data", 1);
    BOOST_CHECK_EQUAL("/custom/data", xdg::BaseDirSpecification::create()->data().home());
    BOOST_CHECK_EQUAL("/custom/data", xdg::data().home());
}

// ---------------------------------------------------------------------------
// XDG_DATA_DIRS
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(XdgDataDirsCorrectlyTokenizesEnv)
{
    ::setenv("XDG_DATA_DIRS", "/tmp:/var", 1);
    auto dirs = xdg::BaseDirSpecification::create()->data().dirs();
    BOOST_CHECK_EQUAL(2u, dirs.size());
    BOOST_CHECK_EQUAL("/tmp", dirs[0]);
    BOOST_CHECK_EQUAL("/var", dirs[1]);
    dirs = xdg::data().dirs();
    BOOST_CHECK_EQUAL(2u, dirs.size());
}

BOOST_AUTO_TEST_CASE(XdgDataDirsIgnoresRelativeEntries)
{
    ::setenv("XDG_DATA_DIRS", "/tmp:tmp:/usr/share", 1);
    auto dirs = xdg::data().dirs();
    BOOST_CHECK_EQUAL(2u, dirs.size());
    BOOST_CHECK_EQUAL("/tmp", dirs[0]);
    BOOST_CHECK_EQUAL("/usr/share", dirs[1]);
}

BOOST_AUTO_TEST_CASE(XdgDataDirsFallsBackWhenAllRelative)
{
    ::setenv("XDG_DATA_DIRS", "tmp:relative", 1);
    auto dirs = xdg::data().dirs();
    BOOST_CHECK_EQUAL(2u, dirs.size());
    BOOST_CHECK_EQUAL("/usr/local/share", dirs[0]);
    BOOST_CHECK_EQUAL("/usr/share", dirs[1]);
}

BOOST_AUTO_TEST_CASE(XdgDataDirsReturnsDefaultValueForEmptyEnv)
{
    ::setenv("XDG_DATA_DIRS", "", 1);
    auto dirs = xdg::data().dirs();
    BOOST_CHECK_EQUAL("/usr/local/share", dirs[0]);
    BOOST_CHECK_EQUAL("/usr/share", dirs[1]);

    dirs = xdg::BaseDirSpecification::create()->data().dirs();
    BOOST_CHECK_EQUAL("/usr/local/share", dirs[0]);
    BOOST_CHECK_EQUAL("/usr/share", dirs[1]);
}

BOOST_AUTO_TEST_CASE(XdgDataDirsIgnoresEmptyComponents)
{
    ::setenv("XDG_DATA_DIRS", "/tmp::/usr/share:", 1);
    auto dirs = xdg::data().dirs();
    BOOST_CHECK_EQUAL(2u, dirs.size());
    BOOST_CHECK_EQUAL("/tmp", dirs[0]);
    BOOST_CHECK_EQUAL("/usr/share", dirs[1]);
}

// ---------------------------------------------------------------------------
// XDG_CONFIG_HOME
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(XdgConfigHomeIgnoresRelativeDirectoryFromEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_CONFIG_HOME", "tmp", 1);
    BOOST_CHECK_EQUAL("/tmp/.config", xdg::BaseDirSpecification::create()->config().home());
    BOOST_CHECK_EQUAL("/tmp/.config", xdg::config().home());
}

BOOST_AUTO_TEST_CASE(XdgConfigHomeReturnsDefaultValueForEmptyEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_CONFIG_HOME", "", 1);
    BOOST_CHECK_EQUAL("/tmp/.config", xdg::BaseDirSpecification::create()->config().home());
    BOOST_CHECK_EQUAL("/tmp/.config", xdg::config().home());
}

BOOST_AUTO_TEST_CASE(XdgConfigHomeUsesAbsoluteEnvValue)
{
    ::setenv("XDG_CONFIG_HOME", "/custom/config", 1);
    BOOST_CHECK_EQUAL("/custom/config", xdg::config().home());
}

// ---------------------------------------------------------------------------
// XDG_CONFIG_DIRS
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(XdgConfigDirsCorrectlyTokenizesEnv)
{
    ::setenv("XDG_CONFIG_DIRS", "/tmp:/etc", 1);
    BOOST_CHECK_EQUAL(2u, xdg::BaseDirSpecification::create()->config().dirs().size());
    BOOST_CHECK_EQUAL(2u, xdg::config().dirs().size());
}

BOOST_AUTO_TEST_CASE(XdgConfigDirsIgnoresRelativeEntries)
{
    ::setenv("XDG_CONFIG_DIRS", "/tmp:tmp:/etc/xdg", 1);
    auto dirs = xdg::config().dirs();
    BOOST_CHECK_EQUAL(2u, dirs.size());
    BOOST_CHECK_EQUAL("/tmp", dirs[0]);
    BOOST_CHECK_EQUAL("/etc/xdg", dirs[1]);
}

BOOST_AUTO_TEST_CASE(XdgConfigDirsFallsBackWhenAllRelative)
{
    ::setenv("XDG_CONFIG_DIRS", "tmp:relative", 1);
    auto dirs = xdg::config().dirs();
    BOOST_CHECK_EQUAL(1u, dirs.size());
    BOOST_CHECK_EQUAL("/etc/xdg", dirs[0]);
}

BOOST_AUTO_TEST_CASE(XdgConfigDirsReturnsDefaultValueForEmptyEnv)
{
    ::setenv("XDG_CONFIG_DIRS", "", 1);
    auto dirs = xdg::config().dirs();
    BOOST_CHECK_EQUAL("/etc/xdg", dirs[0]);
    dirs = xdg::BaseDirSpecification::create()->config().dirs();
    BOOST_CHECK_EQUAL("/etc/xdg", dirs[0]);
}

// ---------------------------------------------------------------------------
// XDG_STATE_HOME
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(XdgStateHomeIgnoresRelativeDirectoryFromEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_STATE_HOME", "tmp", 1);
    BOOST_CHECK_EQUAL("/tmp/.local/state", xdg::BaseDirSpecification::create()->state().home());
    BOOST_CHECK_EQUAL("/tmp/.local/state", xdg::state().home());
}

BOOST_AUTO_TEST_CASE(XdgStateHomeReturnsDefaultValueForEmptyEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_STATE_HOME", "", 1);
    BOOST_CHECK_EQUAL("/tmp/.local/state", xdg::BaseDirSpecification::create()->state().home());
    BOOST_CHECK_EQUAL("/tmp/.local/state", xdg::state().home());
}

// ---------------------------------------------------------------------------
// XDG_CACHE_HOME
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(XdgCacheHomeIgnoresRelativeDirectoryFromEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_CACHE_HOME", "tmp", 1);
    BOOST_CHECK_EQUAL("/tmp/.cache", xdg::BaseDirSpecification::create()->cache().home());
    BOOST_CHECK_EQUAL("/tmp/.cache", xdg::cache().home());
}

BOOST_AUTO_TEST_CASE(XdgCacheHomeReturnsDefaultValueForEmptyEnv)
{
    ::setenv("HOME", "/tmp", 1);
    ::setenv("XDG_CACHE_HOME", "", 1);
    BOOST_CHECK_EQUAL("/tmp/.cache", xdg::BaseDirSpecification::create()->cache().home());
    BOOST_CHECK_EQUAL("/tmp/.cache", xdg::cache().home());
}

// ---------------------------------------------------------------------------
// XDG_RUNTIME_DIR
// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(XdgRuntimeDirThrowsForRelativeDirectoryFromEnv)
{
    ::setenv("XDG_RUNTIME_DIR", "tmp", 1);
    BOOST_CHECK_THROW(xdg::BaseDirSpecification::create()->runtime().dir(), std::runtime_error);
    BOOST_CHECK_THROW(xdg::runtime().dir(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(XdgRuntimeDirThrowsForEmptyEnv)
{
    ::setenv("XDG_RUNTIME_DIR", "", 1);
    BOOST_CHECK_THROW(xdg::BaseDirSpecification::create()->runtime().dir(), std::runtime_error);
    BOOST_CHECK_THROW(xdg::runtime().dir(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(XdgRuntimeDirUsesAbsoluteEnvValue)
{
    ::setenv("XDG_RUNTIME_DIR", "/run/user/1000", 1);
    BOOST_CHECK_EQUAL("/run/user/1000", xdg::runtime().dir());
}
