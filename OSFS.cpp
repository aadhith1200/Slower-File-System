#include "OSFS.hpp"


//class Block

Block::Block()
{
    number = -1;
    frwd = nullptr;
    back = nullptr;
    name = "";
}

Block::Block(int number, string name)
{
    this->name = name;
    this->number = number;
    this->frwd = nullptr;
    this->back = nullptr;
}

Block::~Block()
{
    cout << "block " << this->getNumber() << " is now empty."<< endl;
    reset();
}

int Block::getNumber()
{   return number;  }


void Block::setFrwd(Block* frwd)
{   this->frwd = frwd;  }


void Block::setBack(Block* back)
{   this->back = back;  }


void Block::reset()
{
    number = -1;
    frwd = nullptr;
    back = nullptr;
    name = "";
}

Block* Block::getFrwd()
{   return frwd;    }

Block* Block::getBack()
{   return back;    }

string Block::getName()
{   return name;    }



// class File

File::File(int number, string name): Block(number, name)
{
    for (int i = 0; i < FILE_SIZE; i++) {
        data[i] = '\0';
    }
}

File::~File()
{   cout << "data file " << this->getName() << " is deleted."<< endl;   }


void File::writeFile(int& count, string& input, int& current)
{
    for (; current < FILE_SIZE && 0 < count; count--) {
        if (input != "") {
            data[current] = input[0];
            input = input.substr(input.find(input[0])+1);
            current++;
            cursor++;
        }
        else {
            data[current] = ' ';
            cursor++;
            current++;
        }
    }
}


void File::readFile(int& count, int& current)
{
    for (; current < FILE_SIZE && 0 < count; count--) {
        if (data[current] != '\0') {
            cout << data[current];
            cursor++;
            current++;
        }
        else {
            break;
        }
    }
}


int File::getEnd()
{
    int current;
    bool flag;
    current = 0;
    flag = false;

    for (int i = 0; i < FILE_SIZE; i++) {
        if (data[i] == '\0') {
            current = i;
            flag = true;
            break;
        }
    }

    File* next;
    next = (File*)this->getFrwd();

    if (flag == false && next != nullptr) {
        return next->getEnd()+504;
    }
    else {
        return current;
    }
}


// class Disk

Disk::Disk()
{
    for (int i = 0; i < DISK_SIZE; i++) {
        freeSpaceList[i] = true;
        sector[i] = nullptr;
    }
    close();
}

Disk::~Disk()
{cout << "Disk is now deleted." << endl; }


int Disk::index()
{
    int index;
    index = -1;

    for (int i = 0; i < DISK_SIZE; i++) {
        if (freeSpaceList[i] == true) {
            index = i;
            break;
        }
    }
    return index;
}


bool Disk::isOpen()
{
    bool flag;
    flag = true;
    if (openBlock == -1 && openMode == -1 && cursor == -1) {
        flag = false;
    }
    return flag;
}

void Disk::display()
{
    cout << "*********** File System Display ************" << endl;
    count();
    cout << "Block: ";

    if (openBlock == -1) {
        cout << "NA\t";
    }
    else {
        cout << sector[openBlock]->getName() << endl;
        Block* temp;
        Block* size;
        temp = sector[openBlock]->getFrwd();
        size = sector[openBlock];
        cout << "Block Chain: " << size->getNumber();

        while (temp != nullptr) {
            cout << "<=>" << temp->getNumber();;
            size = temp;
            temp = temp->getFrwd();
        }

        cout << "<=>END" << endl;

    }

    cout << "Cursor: ";
    if (cursor == -1) {
        cout << "NA\t";
    }
    else {
        cout << cursor << "\t";
    }

    cout << "\n********************************************\n" ;
}

void Disk::count()
{
    int free, file;
    free = 0;
    file = 0;
    for (int i = 0; i < DISK_SIZE; i++) {
        if (freeSpaceList[i] == 1) {
            free++;
        }
        else {
            file++;
        }
    }
    cout << "# of free blocks: " << free << endl;
    cout << "# of data file blocks: " << file << endl;
}


void Disk::create(string name)
{
    string original;
    original = name;

        if (isOpen() == true) {
        cout << "Error: Create " << name << " failed because " << sector[openBlock]->getName() << " is opened. Please close it first before creating another file" << endl;
    }
    else {

            Block* newBlock;
            newBlock = helpCreate(name);
            if (newBlock != nullptr) {
                cout << "Finished create " << name << endl;
            }
            else {
                cout << "Error: Create " << name << " failed. ALL the sectors are used." << endl;
            }
        }
}


void Disk::open(string name)
{
    if (isOpen() == true) {
        cout << "Error: Open " << name << " failed because " << sector[openBlock]->getName() << " is opened." << endl;
    }

    else {
        Block* file;
        file = findBlock(name);

        if (file == nullptr) {
            cout << "Error: Open " << name << " failed because file name is not valid. Please try again." << endl;
        }
        else {
            openBlock = file->getNumber();
            cursor = 0;
            openMode = 2;
            seek(-1, 0);
        }
    }

}

void Disk::close()
{
    openBlock = -1;
    openMode = -1;
    cursor = -1;
}

void Disk::write(int count, string input)
{
    if (isOpen() == false) {
        cout << "Error: Write failed because no file is opened." << endl;
    }
    else {
        int current, blockNum;
        File* next;

        current = cursor;
        blockNum = openBlock;
        next = (File*)(sector[openBlock]->getFrwd());


        while (current >= FILE_SIZE && next != nullptr) {
            openBlock = next->getNumber();
            current -= FILE_SIZE;
            next = (File*)(sector[openBlock]->getFrwd());
        }

        if (current >= FILE_SIZE) {
            cout << "Error: Write failed because the cursor is getting beyond the size of " << sector[openBlock]->getName() << endl;
        }
        else {
            helpWrite(count, input, current);
        }
        openBlock = blockNum;
    }
}

void Disk::helpWrite(int& count, string& input, int& current)
{
    Block* newFile;
    int totalToWrite, wrote;

    newFile = sector[openBlock];
    totalToWrite = current + count;
    wrote = 0;

    while (totalToWrite >= FILE_SIZE) {

        current = current % FILE_SIZE;
        if (FILE_SIZE > count) {
            wrote = count - current;
        }
        else {
            wrote = FILE_SIZE - current;
        }

        ((File*)sector[openBlock])->writeFile(count, input, current);
        totalToWrite -= wrote;

        newFile = helpCreate(sector[openBlock]->getName());

        if (newFile != nullptr) {
            sector[openBlock]->setFrwd(newFile);
            newFile->setBack(sector[openBlock]);
            openBlock = newFile->getNumber();
        }
        else {
            break;
        }
    }

    if (totalToWrite < FILE_SIZE && newFile != nullptr) {
        current = current % FILE_SIZE;
        ((File*)sector[openBlock])->writeFile(count, input, current);
        if (count > 0) {
            cout << "Error: Write is unfinished because ALL the sectors are used and the disk can't allocate more." << endl;
        }
    }
    else {
        cout << "Error: Write is unfinished because ALL the sectors are used and the disk can't allocate more." << endl;
    }

}


void Disk::read(int count)
{
    if (isOpen() == false) {
        cout << "Error: Read failed because no file is opened." << endl;
    }
    else {
        int current, blockNum;
        Block* next;

        current = cursor;
        blockNum = openBlock;
        next = sector[openBlock];

        while (current >= FILE_SIZE && next->getFrwd() != nullptr) {
            next = next->getFrwd();
            openBlock = next->getNumber();
            current -= FILE_SIZE;
        }

        if (current >= FILE_SIZE) {
            cout << "Error: Read failed because the cursor is getting beyond the size of " << sector[openBlock]->getName() << endl;
        }
        else {
            int totalRead, readed;
            totalRead = current + count;
            next = sector[openBlock];
            readed = 0;

            while (totalRead >= FILE_SIZE) {
                current = current % FILE_SIZE;
                if (FILE_SIZE > count) {
                    readed = count - current;
                }
                else {
                    readed = FILE_SIZE - current;
                }

                ((File*)sector[openBlock])->readFile(count, current);
                totalRead -= readed;

                next = sector[openBlock]->getFrwd();
                if (next != nullptr) {
                    openBlock = next->getNumber();
                }
                else {
                    break;
                }
            }

            if (totalRead < FILE_SIZE && next != nullptr) {
                current = current % FILE_SIZE;
                ((File*)sector[openBlock])->readFile(count, current);
                cout << "(EOF)" << endl;
                if (count > 0) {
                    cout << "\nEnd of file is reached." << endl;
                }
            }
            else {
                cout << "(EOF)" << endl;
                cout << "\nEnd of file is reached." << endl;
            }
        }
        openBlock = blockNum;
    }
}

void Disk::seek(int base, int offset)
{
    if (isOpen() == false) {
        cout << "Error: Seek failed because no file is opened." << endl;
    }
    else {

        if (base == -1 && offset >= 0) {
            cursor = 0;
            cursor += offset;
        }
        else if (base == -1 && offset < 0) {
            cout << "Error: Seek failed. Can't go backward when reach the beginning of the file." << endl;
        }

        else if (base == 0) {
            if (cursor + offset < 0) {
                cout << "Error: Seek failed. Can't go backward when reach the beginning of the file." << endl;
            }
            else {
                cursor += offset;
            }
        }

        else if (base == 1) {
            if (((File*)sector[openBlock])->getEnd() + offset < 0) {
                cout << "Error: Seek failed. Can't go backward when reach the beginning of the file." << endl;
            }
            else {
                cursor = ((File*)sector[openBlock])->getEnd() + offset;
            }
        }
    }
}


Block* Disk::findBlock(string name)
{
    string original;
    original = name;

    Block* temp;
    temp = nullptr;

    if (1){
        for (int i = 0; i < DISK_SIZE; i++) {
            if (sector[i]!= nullptr && sector[i]->getName() == name) {
                temp = sector[i];
                break;
            }
        }
        return temp;
    }
    else {
        return nullptr;
    }
}

Block* Disk::helpCreate(string name)
{
    Block* newBlock;
    int sectorNum;

    sectorNum = index();
    newBlock = nullptr;

    if (index() != -1) {
        sector[sectorNum] =  new File (sectorNum, name);
        freeSpaceList[sectorNum] = false;
        newBlock = sector[sectorNum];
        cout << "Allocate a new block " << sectorNum << endl;
    }

    return newBlock;
}
















