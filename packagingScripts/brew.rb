
require 'formula'
class OpenCFU < Formula
homepage 'http://opencfu.sourceforge.net/'
url 'h http://sourceforge.net/projects/opencfu/files/linux/opencfu-3.9.0.tar.gz'
sha1 '646561999b6c143701cad7a9c3000aebf973e36f'

depends_on "pkg-config" => :build
depends_on "gtkmm"
depends_on "opencv"

def install
system "./configure" "--prefix=#{prefix}"
system "make install"
end
end
