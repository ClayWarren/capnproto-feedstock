#include "sample.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <kj/io.h>
#include <iostream>
int main() {
    capnp::MallocMessageBuilder message;
    auto sample = message.initRoot<Sample>();
    sample.setName("native-arm64");
    auto values = sample.initValues(3);
    values.set(0, -17); values.set(1, 42); values.set(2, 1234567890123LL);
    kj::VectorOutputStream output;
    capnp::writePackedMessage(output, message);
    kj::ArrayInputStream input(output.getArray());
    capnp::PackedMessageReader reader(input);
    auto result = reader.getRoot<Sample>();
    if (result.getName() != "native-arm64" || result.getValues().size() != 3 ||
        result.getValues()[0] != -17 || result.getValues()[1] != 42 ||
        result.getValues()[2] != 1234567890123LL) return 1;
    std::cout << "Installed schema generation and packed serialization roundtrip passed\n";
}
