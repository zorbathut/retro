
#include "kUniqueId.h"

#include <memory.h>

int kUniqueKeyId::getSize() {
	return KEYUNIQUEIDSIZE; };

const char *kUniqueKeyId::getData() const {
	return uid; };

kUniqueKeyId::kUniqueKeyId() {
	memset( uid, 0, KEYUNIQUEIDSIZE ); };

kUniqueKeyId::kUniqueKeyId( const char *in ) {
	memcpy( uid, in, KEYUNIQUEIDSIZE ); };

int kUniqueDevId::getSize() {
	return DEVUNIQUEIDSIZE; };

const char *kUniqueDevId::getData() const {
	return uid; };

kUniqueDevId::kUniqueDevId() {
	memset( uid, 0, DEVUNIQUEIDSIZE ); };

kUniqueDevId::kUniqueDevId( const char *in ) {
	memcpy( uid, in, DEVUNIQUEIDSIZE ); };


const kUniqueDevId &kUniqueKeyDevId::getDev() const {
	return dev;}
const kUniqueKeyId &kUniqueKeyDevId::getKey() const {
	return key; }

kUniqueKeyDevId::kUniqueKeyDevId() { };
kUniqueKeyDevId::kUniqueKeyDevId( const kUniqueDevId &in_dev, const kUniqueKeyId &in_key ) :
		dev( in_dev ), key( in_key ) { };

bool operator< ( const kUniqueKeyId &a, const kUniqueKeyId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) <  0; };
bool operator<=( const kUniqueKeyId &a, const kUniqueKeyId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) <= 0; };
bool operator> ( const kUniqueKeyId &a, const kUniqueKeyId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) >  0; };
bool operator>=( const kUniqueKeyId &a, const kUniqueKeyId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) >= 0; };
bool operator==( const kUniqueKeyId &a, const kUniqueKeyId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) == 0; };
bool operator!=( const kUniqueKeyId &a, const kUniqueKeyId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) != 0; };

bool operator< ( const kUniqueDevId &a, const kUniqueDevId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) <  0; };
bool operator<=( const kUniqueDevId &a, const kUniqueDevId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) <= 0; };
bool operator> ( const kUniqueDevId &a, const kUniqueDevId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) >  0; };
bool operator>=( const kUniqueDevId &a, const kUniqueDevId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) >= 0; };
bool operator==( const kUniqueDevId &a, const kUniqueDevId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) == 0; };
bool operator!=( const kUniqueDevId &a, const kUniqueDevId &b ) {
	return memcmp( a.getData(), b.getData(), a.getSize() ) != 0; };

bool operator< ( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b ) {
	return a.getDev() < b.getDev() || ( a.getDev() == b.getDev() && a.getKey() <  b.getKey() ); };
bool operator<=( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b ) {
	return a.getDev() < b.getDev() || ( a.getDev() == b.getDev() && a.getKey() <= b.getKey() ); };
bool operator> ( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b ) {
	return a.getDev() > b.getDev() || ( a.getDev() == b.getDev() && a.getKey() >  b.getKey() ); };
bool operator>=( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b ) {
	return a.getDev() > b.getDev() || ( a.getDev() == b.getDev() && a.getKey() >= b.getKey() ); };
bool operator==( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b ) {
	return a.getDev() == b.getDev() && a.getKey() == b.getKey(); };
bool operator!=( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b ) {
	return a.getDev() != b.getDev() || a.getKey() != b.getKey(); };
