#!/usr/bin/perl

use strict;
use warnings;

use lib './gen-perl';

use Thrift;
use Thrift::BinaryProtocol;
use Thrift::Socket;
use Thrift::FramedTransport;

use Data::Dumper;

use Thrudex;

my $socket    = new Thrift::Socket('localhost',9099);
my $transport = new Thrift::FramedTransport($socket);
my $protocol  = new Thrift::BinaryProtocol($transport);
my $client    = new ThrudexClient($protocol);

my $index  = shift;


die "$0 index_name\n" unless defined $index;

eval{

    $transport->open();

    $client->admin("create_index",$index);

    my $d = new Thrudex::Document();
    $d->{index} = $index;
    $d->{key}   = "doc1";

    my $f = new Thrudex::Field();
    $f->{key}   = "field1";
    $f->{value} = "test";

    push(@{$d->{fields}}, $f);


    my $q = new Thrudex::SearchQuery();

    $q->{index}  = $index;
    $q->{query}  = "field1:test";

    my $i = 0;
    while(1){
        my $r = $client->search( $q );
        $i++;

        if($r->total == 2){
            die "Found ".$r->total."\n";
        }elsif($i % 1000){
            print "Found ".$r->total."\n";
        }

        print $client->put($d);
    }


    $transport->close();

}; if($@){
    warn(Dumper($@));
}

