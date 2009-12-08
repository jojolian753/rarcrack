# Copyright 1999-2009 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

# Please create app-crypt/rarcrack directory in
# your local repository and move this file to here.

DESCRIPTION="Rar, Zip, 7zip password recovery tool"
HOMEPAGE="http://rarcrack.sourceforge.net/"
SRC_URI="mirror://sourceforge/rarcrack/${P}.tar.bz2"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~x86"
IUSE=""

DEPEND=">=dev-libs/glib-2.14.3"
RDEPEND="${DEPEND}"

src_compile() {
    emake CFLAGS=${CFLAGS} || die "emake failed"
}

src_install() {
    emake DESTDIR="${D}" PREFIX="/usr" install || die "install failed"
}

pkg_postinst() {
    elog "${P} is installed on your machine! :-D"
    elog "For recovering passwords from 7z/zip/rar archives"
    elog "you should install app-arch/p7zip for 7z, "
    elog "                   app-arch/unrar for rar,"
    elog "                   app-arch/unzip for zip handling."
}

