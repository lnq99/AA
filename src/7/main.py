from bs4 import BeautifulSoup

DATA_DIR = 'data/'
RAW = DATA_DIR + 'raw.html'
PRD = DATA_DIR + 'processed.html'
CSV = DATA_DIR + 'data.csv'
SKIP_LAST_COLUMNS = 6

# 'https://coinmarketcap.com/historical/20210124/'


def processHtml():
    soup = BeautifulSoup(open(RAW), 'html.parser')
    for tag in ('img', 'svg'):
        for i in soup.find_all(tag):
            i.decompose()
    for tag in ('th', 'tr', 'td', 'div', 'a', 'p', 'span'):
        for i in soup.find_all(tag):
            i.attrs.clear()

    with open(PRD, 'w') as f:
        f.write(str(soup))


def saveCSV(n=1000):
    soup = BeautifulSoup(open(PRD), 'html.parser')
    with open(CSV, 'w') as f:
        head = soup.table.thead.tr('th')
        head = [th.text for th in head[:-SKIP_LAST_COLUMNS]]
        bodytr = soup.table.tbody('tr')
        f.write(','.join(head))
        for i in range(n):
            row = [td.text.strip(' \n$').replace(',', '') for td in bodytr[i]('td')[:-SKIP_LAST_COLUMNS]]
            f.write('\n')
            f.write(','.join(row))


if __name__ == '__main__':
    # processHtml()
    saveCSV()