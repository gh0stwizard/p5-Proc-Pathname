use 5.006000;
use ExtUtils::MakeMaker;

my %opts;

WriteMakefile(
    NAME              => 'Proc::Pathname',
    VERSION_FROM      => 'lib/Proc/Pathname.pm',
    PREREQ_PM         => {
        'Test'        =>  0,
        'XSLoader'    =>  0,
    },
    ABSTRACT          => 'retrives the fully qualified path for the program',
    AUTHOR            => 'Vitaliy V. Tokarev <vitaliy.tokarev@gmail.com>',
    LICENSE           => 'perl',
    LIBS              => [''],
    DEFINE            => '',
    INC               => '-I.',
    OBJECT            => '$(O_FILES)',
    %opts
);
