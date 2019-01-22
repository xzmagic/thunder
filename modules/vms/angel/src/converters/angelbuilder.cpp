#include "converters/angelbuilder.h"

#include <log.h>
#include <bson.h>

#include <angelscript.h>

#include <QFile>
#include <QFileInfo>

#include "angelsystem.h"

#define DATA    "Data"

class CBytecodeStream : public asIBinaryStream {
public:
    CBytecodeStream(ByteArray &ptr) :
        array(ptr) {

    }
    int Write(const void *ptr, asUINT size) {
        if( size == 0 ) {
            return size;
        }
        uint32_t offset = array.size();
        array.resize(offset + size);
        memcpy(&array[offset], ptr, size);

        return size;
    }
    int Read(void *ptr, asUINT size) {
        return 0;
    }
protected:
    ByteArray        &array;
};

VariantMap AngelSerial::saveUserData() const {
    VariantMap result;

    result[DATA]  = m_Array;

    return result;
}

AngelBuilder::AngelBuilder() {
    m_pScriptEngine = asCreateScriptEngine();

    m_pScriptEngine->SetMessageCallback(asFUNCTION(messageCallback), 0, asCALL_CDECL);

    AngelSystem::registerClasses(m_pScriptEngine);
}

bool AngelBuilder::buildProject() {
    if(m_Outdated) {
        asIScriptModule *mod = m_pScriptEngine->GetModule("module", asGM_CREATE_IF_NOT_EXISTS);
        foreach(QString it, m_Sources) {
            QFile file(it);
            if(file.open( QIODevice::ReadOnly)) {
                mod->AddScriptSection("AngelData", file.readAll().data());
                file.close();
            }
        }
        if(mod->Build() >= 0) {
            QFile dst(m_Destination);
            if(dst.open( QIODevice::WriteOnly)) {
                AngelSerial serial;
                serial.m_Array.clear();
                CBytecodeStream stream(serial.m_Array);
                mod->SaveByteCode(&stream);

                ByteArray data = Bson::save( Engine::toVariant(&serial) );
                dst.write((const char *)&data[0], data.size());
                dst.close();
            }
        }

        m_Outdated = false;
    }
    return true;
}
#include <QDebug>
uint8_t AngelBuilder::convertFile(IConverterSettings *settings) {
    QFileInfo info(settings->absoluteDestination());

    m_Destination = info.absolutePath() + "/{00000000-0101-0000-0000-000000000000}";

    qDebug() << m_Destination;

    return IBuilder::convertFile(settings);
}

QString AngelBuilder::builderVersion() {
    return "1.0";
}

void AngelBuilder::messageCallback(const asSMessageInfo *msg, void *param) {
    A_UNUSED(param)
    Log((Log::LogTypes)msg->type) << msg->section << "(line:" << msg->row << "col:" << msg->col << "):" << msg->message;
}