import { service } from '@/utils/request'

// 导航接口
export const Navigate = (startId, endId) => {
    const url = '/navigate/path?' + 'from_id=' + startId + '&' + 'des_id=' + endId

    return service({
        url: url,
        method: 'get',
    })
}

// tsp问题接口
export const TSP = (startId, passIds) => {
    const url = '/navigate/tsp?' + 'start_id=' + startId + '&' + 'pass_ids=' + passIds

    return service({
        url: url,
        method: 'get',
    })
}
