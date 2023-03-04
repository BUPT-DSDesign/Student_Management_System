import { service } from '@/utils/request'

// 导航接口
export const Navigate = (startId, endId) => {
    const url = '/navigate/path?' + 'from_id=' + startId + '&' + 'des_id=' + endId

    return service({
        url: url,
        method: 'get',
    })
}