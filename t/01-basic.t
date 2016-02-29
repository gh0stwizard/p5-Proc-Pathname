use strict;
use warnings;

use Test;
use Encode;
BEGIN { plan tests => 2 };
use Proc::Pathname;

ok(1);
ok(defined Proc::Pathname::get());
